/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "SDL_internal.h"

#ifdef SDL_VIDEO_DRIVER_WAYLAND

#include "../SDL_sysvideo.h"
#include "../SDL_video_c.h"

#include "../../events/SDL_mouse_c.h"
#include "SDL_waylandvideo.h"
#include "../SDL_pixels_c.h"
#include "SDL_waylandevents_c.h"

#include "wayland-cursor.h"
#include "SDL_waylandmouse.h"
#include "SDL_waylandshmbuffer.h"

#include "cursor-shape-v1-client-protocol.h"
#include "pointer-constraints-unstable-v1-client-protocol.h"
#include "viewporter-client-protocol.h"

#include "../../SDL_hints_c.h"

static SDL_Cursor *sys_cursors[SDL_HITTEST_RESIZE_LEFT + 1];

static bool Wayland_SetRelativeMouseMode(bool enabled);

typedef struct
{
    struct Wayland_SHMBuffer shmBuffer;

    int dst_width;
    int dst_height;
    double scale;

    struct wl_list node;
} Wayland_CachedCustomCursor;

typedef struct
{
    SDL_Surface *sdl_cursor_surface;
    struct wl_list cursor_cache;
} Wayland_CustomCursor;

typedef struct
{
    struct wl_buffer *wl_buffer;
    Uint32 duration;
} Wayland_SystemCursorFrame;

typedef struct
{
    Wayland_SystemCursorFrame *frames;
    struct wl_callback *frame_callback;
    Uint64 last_frame_callback_time_ms;
    Uint64 current_frame_time_ms;
    Uint32 total_duration;
    int num_frames;
    int current_frame;
    SDL_SystemCursor id;
} Wayland_SystemCursor;

struct SDL_CursorData
{
    union
    {
        Wayland_CustomCursor custom;
        Wayland_SystemCursor system;
    } cursor_data;

    struct wl_surface *surface;
    struct wp_viewport *viewport;

    int hot_x, hot_y;

    bool is_system_cursor;
};

static int dbus_cursor_size;
static char *dbus_cursor_theme;

static void Wayland_FreeCursorThemes(SDL_VideoData *vdata)
{
    for (int i = 0; i < vdata->num_cursor_themes; i += 1) {
        WAYLAND_wl_cursor_theme_destroy(vdata->cursor_themes[i].theme);
    }
    vdata->num_cursor_themes = 0;
    SDL_free(vdata->cursor_themes);
    vdata->cursor_themes = NULL;
}

#ifdef SDL_USE_LIBDBUS

#include "../../core/linux/SDL_dbus.h"

#define CURSOR_NODE        "org.freedesktop.portal.Desktop"
#define CURSOR_PATH        "/org/freedesktop/portal/desktop"
#define CURSOR_INTERFACE   "org.freedesktop.portal.Settings"
#define CURSOR_NAMESPACE   "org.gnome.desktop.interface"
#define CURSOR_SIGNAL_NAME "SettingChanged"
#define CURSOR_SIZE_KEY    "cursor-size"
#define CURSOR_THEME_KEY   "cursor-theme"

static DBusMessage *Wayland_ReadDBusProperty(SDL_DBusContext *dbus, const char *key)
{
    static const char *iface = "org.gnome.desktop.interface";

    DBusMessage *reply = NULL;
    DBusMessage *msg = dbus->message_new_method_call(CURSOR_NODE,
                                                     CURSOR_PATH,
                                                     CURSOR_INTERFACE,
                                                     "Read"); // Method

    if (msg) {
        if (dbus->message_append_args(msg, DBUS_TYPE_STRING, &iface, DBUS_TYPE_STRING, &key, DBUS_TYPE_INVALID)) {
            reply = dbus->connection_send_with_reply_and_block(dbus->session_conn, msg, DBUS_TIMEOUT_USE_DEFAULT, NULL);
        }
        dbus->message_unref(msg);
    }

    return reply;
}

static bool Wayland_ParseDBusReply(SDL_DBusContext *dbus, DBusMessage *reply, int type, void *value)
{
    DBusMessageIter iter[3];

    dbus->message_iter_init(reply, &iter[0]);
    if (dbus->message_iter_get_arg_type(&iter[0]) != DBUS_TYPE_VARIANT) {
        return false;
    }

    dbus->message_iter_recurse(&iter[0], &iter[1]);
    if (dbus->message_iter_get_arg_type(&iter[1]) != DBUS_TYPE_VARIANT) {
        return false;
    }

    dbus->message_iter_recurse(&iter[1], &iter[2]);
    if (dbus->message_iter_get_arg_type(&iter[2]) != type) {
        return false;
    }

    dbus->message_iter_get_basic(&iter[2], value);

    return true;
}

static DBusHandlerResult Wayland_DBusCursorMessageFilter(DBusConnection *conn, DBusMessage *msg, void *data)
{
    SDL_DBusContext *dbus = SDL_DBus_GetContext();
    SDL_VideoData *vdata = (SDL_VideoData *)data;

    if (dbus->message_is_signal(msg, CURSOR_INTERFACE, CURSOR_SIGNAL_NAME)) {
        DBusMessageIter signal_iter, variant_iter;
        const char *namespace, *key;

        dbus->message_iter_init(msg, &signal_iter);
        // Check if the parameters are what we expect
        if (dbus->message_iter_get_arg_type(&signal_iter) != DBUS_TYPE_STRING) {
            goto not_our_signal;
        }
        dbus->message_iter_get_basic(&signal_iter, &namespace);
        if (SDL_strcmp(CURSOR_NAMESPACE, namespace) != 0) {
            goto not_our_signal;
        }
        if (!dbus->message_iter_next(&signal_iter)) {
            goto not_our_signal;
        }
        if (dbus->message_iter_get_arg_type(&signal_iter) != DBUS_TYPE_STRING) {
            goto not_our_signal;
        }
        dbus->message_iter_get_basic(&signal_iter, &key);
        if (SDL_strcmp(CURSOR_SIZE_KEY, key) == 0) {
            int new_cursor_size;

            if (!dbus->message_iter_next(&signal_iter)) {
                goto not_our_signal;
            }
            if (dbus->message_iter_get_arg_type(&signal_iter) != DBUS_TYPE_VARIANT) {
                goto not_our_signal;
            }
            dbus->message_iter_recurse(&signal_iter, &variant_iter);
            if (dbus->message_iter_get_arg_type(&variant_iter) != DBUS_TYPE_INT32) {
                goto not_our_signal;
            }
            dbus->message_iter_get_basic(&variant_iter, &new_cursor_size);

            if (dbus_cursor_size != new_cursor_size) {
                dbus_cursor_size = new_cursor_size;
                SDL_SetCursor(NULL); // Force cursor update
            }
        } else if (SDL_strcmp(CURSOR_THEME_KEY, key) == 0) {
            const char *new_cursor_theme = NULL;

            if (!dbus->message_iter_next(&signal_iter)) {
                goto not_our_signal;
            }
            if (dbus->message_iter_get_arg_type(&signal_iter) != DBUS_TYPE_VARIANT) {
                goto not_our_signal;
            }
            dbus->message_iter_recurse(&signal_iter, &variant_iter);
            if (dbus->message_iter_get_arg_type(&variant_iter) != DBUS_TYPE_STRING) {
                goto not_our_signal;
            }
            dbus->message_iter_get_basic(&variant_iter, &new_cursor_theme);

            if (!dbus_cursor_theme || !new_cursor_theme || SDL_strcmp(dbus_cursor_theme, new_cursor_theme) != 0) {
                SDL_free(dbus_cursor_theme);
                if (new_cursor_theme) {
                    dbus_cursor_theme = SDL_strdup(new_cursor_theme);
                } else {
                    dbus_cursor_theme = NULL;
                }

                // Purge the current cached themes and force a cursor refresh.
                Wayland_FreeCursorThemes(vdata);
                SDL_SetCursor(NULL);
            }
        } else {
            goto not_our_signal;
        }

        return DBUS_HANDLER_RESULT_HANDLED;
    }

not_our_signal:
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

static void Wayland_DBusInitCursorProperties(SDL_VideoData *vdata)
{
    DBusMessage *reply;
    SDL_DBusContext *dbus = SDL_DBus_GetContext();
    bool add_filter = false;

    if (!dbus) {
        return;
    }

    if ((reply = Wayland_ReadDBusProperty(dbus, CURSOR_SIZE_KEY))) {
        if (Wayland_ParseDBusReply(dbus, reply, DBUS_TYPE_INT32, &dbus_cursor_size)) {
            add_filter = true;
        }
        dbus->message_unref(reply);
    }

    if ((reply = Wayland_ReadDBusProperty(dbus, CURSOR_THEME_KEY))) {
        const char *temp = NULL;
        if (Wayland_ParseDBusReply(dbus, reply, DBUS_TYPE_STRING, &temp)) {
            add_filter = true;

            if (temp) {
                dbus_cursor_theme = SDL_strdup(temp);
            }
        }
        dbus->message_unref(reply);
    }

    // Only add the filter if at least one of the settings we want is present.
    if (add_filter) {
        dbus->bus_add_match(dbus->session_conn,
                            "type='signal', interface='" CURSOR_INTERFACE "',"
                            "member='" CURSOR_SIGNAL_NAME "', arg0='" CURSOR_NAMESPACE "'",
                            NULL);
        dbus->connection_add_filter(dbus->session_conn, &Wayland_DBusCursorMessageFilter, vdata, NULL);
        dbus->connection_flush(dbus->session_conn);
    }
}

static void Wayland_DBusFinishCursorProperties(void)
{
    SDL_free(dbus_cursor_theme);
    dbus_cursor_theme = NULL;
}

#endif

static void cursor_frame_done(void *data, struct wl_callback *cb, uint32_t time);
struct wl_callback_listener cursor_frame_listener = {
    cursor_frame_done
};

static void cursor_frame_done(void *data, struct wl_callback *cb, uint32_t time)
{
    SDL_CursorData *c = (SDL_CursorData *)data;

    const Uint64 now = SDL_GetTicks();
    const Uint64 elapsed = (now - c->cursor_data.system.last_frame_callback_time_ms) % c->cursor_data.system.total_duration;
    Uint64 advance = 0;
    int next = c->cursor_data.system.current_frame;

    wl_callback_destroy(cb);
    c->cursor_data.system.frame_callback = wl_surface_frame(c->surface);
    wl_callback_add_listener(c->cursor_data.system.frame_callback, &cursor_frame_listener, data);

    c->cursor_data.system.current_frame_time_ms += elapsed;

    // Calculate the next frame based on the elapsed duration.
    for (Uint64 t = c->cursor_data.system.frames[next].duration; t <= c->cursor_data.system.current_frame_time_ms; t += c->cursor_data.system.frames[next].duration) {
        next = (next + 1) % c->cursor_data.system.num_frames;
        advance = t;

        // Make sure we don't end up in an infinite loop if a cursor has frame durations of 0.
        if (!c->cursor_data.system.frames[next].duration) {
            break;
        }
    }

    c->cursor_data.system.current_frame_time_ms -= advance;
    c->cursor_data.system.last_frame_callback_time_ms = now;
    c->cursor_data.system.current_frame = next;
    wl_surface_attach(c->surface, c->cursor_data.system.frames[next].wl_buffer, 0, 0);
    if (wl_surface_get_version(c->surface) >= WL_SURFACE_DAMAGE_BUFFER_SINCE_VERSION) {
        wl_surface_damage_buffer(c->surface, 0, 0, SDL_MAX_SINT32, SDL_MAX_SINT32);
    } else {
        wl_surface_damage(c->surface, 0, 0, SDL_MAX_SINT32, SDL_MAX_SINT32);
    }
    wl_surface_commit(c->surface);
}

static bool wayland_get_system_cursor(SDL_VideoData *vdata, SDL_CursorData *cdata, double *scale)
{
    struct wl_cursor_theme *theme = NULL;
    struct wl_cursor *cursor;
    const char *css_name = "default";
    const char *fallback_name = NULL;

    int size = dbus_cursor_size;

    SDL_Window *focus;

    // Fallback envvar if the DBus properties don't exist
    if (size <= 0) {
        const char *xcursor_size = SDL_getenv("XCURSOR_SIZE");
        if (xcursor_size) {
            size = SDL_atoi(xcursor_size);
        }
    }
    if (size <= 0) {
        size = 24;
    }
    // First, find the appropriate theme based on the current scale...
    focus = SDL_GetMouse()->focus;
    if (focus) {
        // TODO: Use the fractional scale once GNOME supports viewports on cursor surfaces.
        *scale = SDL_ceil(focus->internal->scale_factor);
    } else {
        *scale = 1.0;
    }

    size *= (int)*scale;
    for (int i = 0; i < vdata->num_cursor_themes; ++i) {
        if (vdata->cursor_themes[i].size == size) {
            theme = vdata->cursor_themes[i].theme;
            break;
        }
    }
    if (!theme) {
        const char *xcursor_theme = dbus_cursor_theme;

        SDL_WaylandCursorTheme *new_cursor_themes = SDL_realloc(vdata->cursor_themes,
                                                                sizeof(SDL_WaylandCursorTheme) * (vdata->num_cursor_themes + 1));
        if (!new_cursor_themes) {
            return false;
        }
        vdata->cursor_themes = new_cursor_themes;

        // Fallback envvar if the DBus properties don't exist
        if (!xcursor_theme) {
            xcursor_theme = SDL_getenv("XCURSOR_THEME");
        }

        theme = WAYLAND_wl_cursor_theme_load(xcursor_theme, size, vdata->shm);
        vdata->cursor_themes[vdata->num_cursor_themes].size = size;
        vdata->cursor_themes[vdata->num_cursor_themes++].theme = theme;
    }

    css_name = SDL_GetCSSCursorName(cdata->cursor_data.system.id, &fallback_name);
    cursor = WAYLAND_wl_cursor_theme_get_cursor(theme, css_name);
    if (!cursor && fallback_name) {
        cursor = WAYLAND_wl_cursor_theme_get_cursor(theme, fallback_name);
    }

    // Fallback to the default cursor if the chosen one wasn't found
    if (!cursor) {
        cursor = WAYLAND_wl_cursor_theme_get_cursor(theme, "default");
    }
    // Try the old X11 name as a last resort
    if (!cursor) {
        cursor = WAYLAND_wl_cursor_theme_get_cursor(theme, "left_ptr");
    }
    if (!cursor) {
        return false;
    }

    if (cdata->cursor_data.system.num_frames != cursor->image_count) {
        SDL_free(cdata->cursor_data.system.frames);
        cdata->cursor_data.system.frames = SDL_calloc(cursor->image_count, sizeof(Wayland_SystemCursorFrame));
        if (!cdata->cursor_data.system.frames) {
            return false;
        }
    }

    // ... Set the cursor data, finally.
    cdata->cursor_data.system.num_frames = cursor->image_count;
    cdata->cursor_data.system.total_duration = 0;
    for (int i = 0; i < cursor->image_count; ++i) {
        cdata->cursor_data.system.frames[i].wl_buffer = WAYLAND_wl_cursor_image_get_buffer(cursor->images[i]);
        cdata->cursor_data.system.frames[i].duration = cursor->images[i]->delay;
        cdata->cursor_data.system.total_duration += cursor->images[i]->delay;
    }
    cdata->hot_x = cursor->images[0]->hotspot_x;
    cdata->hot_y = cursor->images[0]->hotspot_y;
    return true;
}

static Wayland_CachedCustomCursor *Wayland_GetCachedCustomCursor(SDL_Cursor *cursor)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *wd = vd->internal;
    SDL_CursorData *data = cursor->internal;
    Wayland_CachedCustomCursor *cache;
    SDL_Window *focus = SDL_GetMouseFocus();
    double scale = 1.0;

    if (focus && SDL_SurfaceHasAlternateImages(data->cursor_data.custom.sdl_cursor_surface)) {
        scale = focus->internal->scale_factor;
    }

    // Only use fractional scale values if viewports are available.
    if (!wd->viewporter) {
        scale = SDL_ceil(scale);
    }

    // Is this cursor already cached at the target scale?
    wl_list_for_each(cache, &data->cursor_data.custom.cursor_cache, node) {
        if (cache->scale == scale) {
            return cache;
        }
    }

    cache = SDL_calloc(1, sizeof(Wayland_CachedCustomCursor));
    if (!cache) {
        return NULL;
    }

    SDL_Surface *surface = SDL_GetSurfaceImage(data->cursor_data.custom.sdl_cursor_surface, (float)scale);
    if (!surface) {
        SDL_free(cache);
        return NULL;
    }

    // Allocate shared memory buffer for this cursor
    if (!Wayland_AllocSHMBuffer(surface->w, surface->h, &cache->shmBuffer)) {
        SDL_free(cache);
        SDL_DestroySurface(surface);
        return NULL;
    }

    // Wayland requires premultiplied alpha for its surfaces.
    SDL_PremultiplyAlpha(surface->w, surface->h,
                         surface->format, surface->pixels, surface->pitch,
                         SDL_PIXELFORMAT_ARGB8888, cache->shmBuffer.shm_data, surface->w * 4, true);

    cache->dst_width = data->cursor_data.custom.sdl_cursor_surface->w;
    cache->dst_height = data->cursor_data.custom.sdl_cursor_surface->h;
    cache->scale = scale;
    WAYLAND_wl_list_insert(&data->cursor_data.custom.cursor_cache, &cache->node);
    SDL_DestroySurface(surface);

    return cache;
}

static SDL_Cursor *Wayland_CreateCursor(SDL_Surface *surface, int hot_x, int hot_y)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *wd = vd->internal;

    SDL_Cursor *cursor = SDL_calloc(1, sizeof(*cursor));
    if (cursor) {
        SDL_CursorData *data = SDL_calloc(1, sizeof(*data));
        if (!data) {
            SDL_free(cursor);
            return NULL;
        }
        cursor->internal = data;
        WAYLAND_wl_list_init(&data->cursor_data.custom.cursor_cache);
        data->hot_x = hot_x;
        data->hot_y = hot_y;
        data->surface = wl_compositor_create_surface(wd->compositor);
        wl_surface_set_user_data(data->surface, NULL);

        data->cursor_data.custom.sdl_cursor_surface = surface;
        ++surface->refcount;

        // If the cursor has only one size, just prepare it now.
        if (!SDL_SurfaceHasAlternateImages(surface)) {
            Wayland_GetCachedCustomCursor(cursor);
        }
    }

    return cursor;
}

static SDL_Cursor *Wayland_CreateSystemCursor(SDL_SystemCursor id)
{
    SDL_VideoData *data = SDL_GetVideoDevice()->internal;
    SDL_Cursor *cursor = SDL_calloc(1, sizeof(*cursor));
    if (cursor) {
        SDL_CursorData *cdata = SDL_calloc(1, sizeof(*cdata));
        if (!cdata) {
            SDL_free(cursor);
            return NULL;
        }
        cursor->internal = cdata;

        /* The surface is only necessary if the cursor shape manager is not present.
         *
         * Note that we can't actually set any other cursor properties, as this
         * is output-specific. See wayland_get_system_cursor for the rest!
         */
        if (!data->cursor_shape_manager) {
            cdata->surface = wl_compositor_create_surface(data->compositor);
            wl_surface_set_user_data(cdata->surface, NULL);
        }

        cdata->cursor_data.system.id = id;
        cdata->is_system_cursor = true;
    }

    return cursor;
}

static SDL_Cursor *Wayland_CreateDefaultCursor(void)
{
    return Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
}

static void Wayland_FreeCursorData(SDL_CursorData *d)
{
    // Buffers for system cursors must not be destroyed.
    if (d->is_system_cursor) {
        if (d->cursor_data.system.frame_callback) {
            wl_callback_destroy(d->cursor_data.system.frame_callback);
        }
        SDL_free(d->cursor_data.system.frames);
    } else {
        Wayland_CachedCustomCursor *c, *temp;
        wl_list_for_each_safe(c, temp, &d->cursor_data.custom.cursor_cache, node) {
            Wayland_ReleaseSHMBuffer(&c->shmBuffer);
            SDL_free(c);
        }

        SDL_DestroySurface(d->cursor_data.custom.sdl_cursor_surface);
    }

    if (d->viewport) {
        wp_viewport_destroy(d->viewport);
        d->viewport = NULL;
    }

    if (d->surface) {
        wl_surface_destroy(d->surface);
        d->surface = NULL;
    }
}

static void Wayland_FreeCursor(SDL_Cursor *cursor)
{
    if (!cursor) {
        return;
    }

    // Probably not a cursor we own
    if (!cursor->internal) {
        return;
    }

    Wayland_FreeCursorData(cursor->internal);

    SDL_free(cursor->internal);
    SDL_free(cursor);
}

static void Wayland_SetSystemCursorShape(struct SDL_WaylandInput *input, SDL_SystemCursor id)
{
    Uint32 shape;

    switch (id) {
    case SDL_SYSTEM_CURSOR_DEFAULT:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT;
        break;
    case SDL_SYSTEM_CURSOR_TEXT:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_TEXT;
        break;
    case SDL_SYSTEM_CURSOR_WAIT:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_WAIT;
        break;
    case SDL_SYSTEM_CURSOR_CROSSHAIR:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_CROSSHAIR;
        break;
    case SDL_SYSTEM_CURSOR_PROGRESS:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_PROGRESS;
        break;
    case SDL_SYSTEM_CURSOR_NWSE_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NWSE_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_NESW_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NESW_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_EW_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_EW_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_NS_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NS_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_MOVE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ALL_SCROLL;
        break;
    case SDL_SYSTEM_CURSOR_NOT_ALLOWED:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NOT_ALLOWED;
        break;
    case SDL_SYSTEM_CURSOR_POINTER:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_POINTER;
        break;
    case SDL_SYSTEM_CURSOR_NW_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NW_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_N_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_N_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_NE_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NE_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_E_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_E_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_SE_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_SE_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_S_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_S_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_SW_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_SW_RESIZE;
        break;
    case SDL_SYSTEM_CURSOR_W_RESIZE:
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_W_RESIZE;
        break;
    default:
        SDL_assert(0); // Should never be here...
        shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT;
    }

    wp_cursor_shape_device_v1_set_shape(input->cursor_shape, input->pointer_enter_serial, shape);
}

static bool Wayland_ShowCursor(SDL_Cursor *cursor)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->internal;
    struct SDL_WaylandInput *input = d->input;
    struct wl_pointer *pointer = d->pointer;
    double scale = 1.0;
    int dst_width = 0;
    int dst_height = 0;

    if (!pointer) {
        return false;
    }

    // Stop the frame callback for old animated cursors.
    if (input->current_cursor && input->current_cursor->is_system_cursor &&
        input->current_cursor->cursor_data.system.frame_callback) {
        wl_callback_destroy(input->current_cursor->cursor_data.system.frame_callback);
        input->current_cursor->cursor_data.system.frame_callback = NULL;
    }

    if (cursor) {
        SDL_CursorData *data = cursor->internal;

        if (data->is_system_cursor) {
            if (input->cursor_shape) {
                Wayland_SetSystemCursorShape(input, data->cursor_data.system.id);
                input->current_cursor = data;

                return true;
            }

            if (!wayland_get_system_cursor(d, data, &scale)) {
                return false;
            }
        }

        if (data->is_system_cursor) {
            wl_surface_attach(data->surface, data->cursor_data.system.frames[0].wl_buffer, 0, 0);

            // If more than one frame is available, create a frame callback to run the animation.
            if (data->cursor_data.system.num_frames > 1) {
                data->cursor_data.system.last_frame_callback_time_ms = SDL_GetTicks();
                data->cursor_data.system.current_frame_time_ms = 0;
                data->cursor_data.system.current_frame = 0;
                data->cursor_data.system.frame_callback = wl_surface_frame(data->surface);
                wl_callback_add_listener(data->cursor_data.system.frame_callback, &cursor_frame_listener, data);
            }
        } else {
            Wayland_CachedCustomCursor *cached = Wayland_GetCachedCustomCursor(cursor);
            if (!cached) {
                return false;
            }
            dst_width = cached->dst_width;
            dst_height = cached->dst_height;
            scale = cached->scale;
            wl_surface_attach(data->surface, cached->shmBuffer.wl_buffer, 0, 0);
        }

        // TODO: Make the viewport path the default in all cases once GNOME finally supports viewports on cursor surfaces.
        if (SDL_ceil(scale) != scale && d->viewporter) {
            if (!data->viewport) {
                data->viewport = wp_viewporter_get_viewport(d->viewporter, data->surface);
            }
            wl_surface_set_buffer_scale(data->surface, 1);
            wp_viewport_set_source(data->viewport, wl_fixed_from_int(-1), wl_fixed_from_int(-1), wl_fixed_from_int(-1), wl_fixed_from_int(-1));
            wp_viewport_set_destination(data->viewport, dst_width, dst_height);
            wl_pointer_set_cursor(pointer, input->pointer_enter_serial, data->surface, data->hot_x, data->hot_y);
        } else {
            if (data->viewport) {
                wp_viewport_destroy(data->viewport);
                data->viewport = NULL;
            }
            wl_surface_set_buffer_scale(data->surface, (int32_t)scale);
            wl_pointer_set_cursor(pointer, input->pointer_enter_serial, data->surface, (int32_t)(data->hot_x / scale), (int32_t)(data->hot_y / scale));
        }

        if (wl_surface_get_version(data->surface) >= WL_SURFACE_DAMAGE_BUFFER_SINCE_VERSION) {
            wl_surface_damage_buffer(data->surface, 0, 0, SDL_MAX_SINT32, SDL_MAX_SINT32);
        } else {
            wl_surface_damage(data->surface, 0, 0, SDL_MAX_SINT32, SDL_MAX_SINT32);
        }

        wl_surface_commit(data->surface);
        input->current_cursor = data;
    } else {
        input->current_cursor = NULL;
        wl_pointer_set_cursor(pointer, input->pointer_enter_serial, NULL, 0, 0);
    }

    return true;
}

static bool Wayland_WarpMouse(SDL_Window *window, float x, float y)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->internal;
    SDL_WindowData *wind = window->internal;
    struct SDL_WaylandInput *input = d->input;

    if (d->pointer_constraints) {
        const bool toggle_lock = !wind->locked_pointer;

        /* The pointer confinement protocol allows setting a hint to warp the pointer,
         * but only when the pointer is locked.
         *
         * Lock the pointer, set the position hint, unlock, and hope for the best.
         */
        if (toggle_lock) {
            Wayland_input_lock_pointer(input, window);
        }
        if (wind->locked_pointer) {
            const wl_fixed_t f_x = wl_fixed_from_double(x / wind->pointer_scale.x);
            const wl_fixed_t f_y = wl_fixed_from_double(y / wind->pointer_scale.y);
            zwp_locked_pointer_v1_set_cursor_position_hint(wind->locked_pointer, f_x, f_y);
            wl_surface_commit(wind->surface);
        }
        if (toggle_lock) {
            Wayland_input_unlock_pointer(input, window);
        }

        /* NOTE: There is a pending warp event under discussion that should replace this when available.
         * https://gitlab.freedesktop.org/wayland/wayland/-/merge_requests/340
         */
        SDL_SendMouseMotion(0, window, SDL_GLOBAL_MOUSE_ID, false, x, y);
    } else {
        return SDL_SetError("wayland: mouse warp failed; compositor lacks support for the required zwp_pointer_confinement_v1 protocol");
    }

    return true;
}

static bool Wayland_WarpMouseGlobal(float x, float y)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->internal;
    struct SDL_WaylandInput *input = d->input;
    SDL_WindowData *wind = input->pointer_focus;

    // If the client wants the coordinates warped to within the focused window, just convert the coordinates to relative.
    if (wind) {
        SDL_Window *window = wind->sdlwindow;
        return Wayland_WarpMouse(window, x - (float)window->x, y - (float)window->y);
    }

    return SDL_SetError("wayland: can't warp the mouse when a window does not have focus");
}

static bool Wayland_SetRelativeMouseMode(bool enabled)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *data = vd->internal;

    if (enabled) {
        return Wayland_input_enable_relative_pointer(data->input);
    } else {
        return Wayland_input_disable_relative_pointer(data->input);
    }
}

/* Wayland doesn't support getting the true global cursor position, but it can
 * be faked well enough for what most applications use it for: querying the
 * global cursor coordinates and transforming them to the window-relative
 * coordinates manually.
 *
 * The global position is derived by taking the cursor position relative to the
 * toplevel window, and offsetting it by the origin of the output the window is
 * currently considered to be on. The cursor position and button state when the
 * cursor is outside an application window are unknown, but this gives 'correct'
 * coordinates when the window has focus, which is good enough for most
 * applications.
 */
static SDL_MouseButtonFlags SDLCALL Wayland_GetGlobalMouseState(float *x, float *y)
{
    SDL_Window *focus = SDL_GetMouseFocus();
    SDL_MouseButtonFlags result = 0;

    if (focus) {
        int off_x, off_y;

        result = SDL_GetMouseState(x, y);
        SDL_RelativeToGlobalForWindow(focus, focus->x, focus->y, &off_x, &off_y);
        *x += off_x;
        *y += off_y;
    }

    return result;
}

#if 0  // TODO RECONNECT: See waylandvideo.c for more information!
static void Wayland_RecreateCursor(SDL_Cursor *cursor, SDL_VideoData *vdata)
{
    SDL_CursorData *cdata = cursor->internal;

    // Probably not a cursor we own
    if (cdata == NULL) {
        return;
    }

    Wayland_FreeCursorData(cdata);

    // We're not currently freeing this, so... yolo?
    if (cdata->shm_data != NULL) {
        void *old_data_pointer = cdata->shm_data;
        int stride = cdata->w * 4;

        create_buffer_from_shm(cdata, cdata->w, cdata->h, WL_SHM_FORMAT_ARGB8888);

        SDL_memcpy(cdata->shm_data, old_data_pointer, stride * cdata->h);
    }
    cdata->surface = wl_compositor_create_surface(vdata->compositor);
    wl_surface_set_user_data(cdata->surface, NULL);
}

void Wayland_RecreateCursors(void)
{
    SDL_Cursor *cursor;
    SDL_Mouse *mouse = SDL_GetMouse();
    SDL_VideoData *vdata = SDL_GetVideoDevice()->internal;

    if (vdata && vdata->cursor_themes) {
        SDL_free(vdata->cursor_themes);
        vdata->cursor_themes = NULL;
        vdata->num_cursor_themes = 0;
    }

    if (mouse == NULL) {
        return;
    }

    for (cursor = mouse->cursors; cursor != NULL; cursor = cursor->next) {
        Wayland_RecreateCursor(cursor, vdata);
    }
    if (mouse->def_cursor) {
        Wayland_RecreateCursor(mouse->def_cursor, vdata);
    }
    if (mouse->cur_cursor) {
        Wayland_RecreateCursor(mouse->cur_cursor, vdata);
        if (mouse->cursor_shown) {
            Wayland_ShowCursor(mouse->cur_cursor);
        }
    }
}
#endif // 0

void Wayland_InitMouse(void)
{
    SDL_Mouse *mouse = SDL_GetMouse();
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->internal;

    mouse->CreateCursor = Wayland_CreateCursor;
    mouse->CreateSystemCursor = Wayland_CreateSystemCursor;
    mouse->ShowCursor = Wayland_ShowCursor;
    mouse->FreeCursor = Wayland_FreeCursor;
    mouse->WarpMouse = Wayland_WarpMouse;
    mouse->WarpMouseGlobal = Wayland_WarpMouseGlobal;
    mouse->SetRelativeMouseMode = Wayland_SetRelativeMouseMode;
    mouse->GetGlobalMouseState = Wayland_GetGlobalMouseState;

    SDL_HitTestResult r = SDL_HITTEST_NORMAL;
    while (r <= SDL_HITTEST_RESIZE_LEFT) {
        switch (r) {
        case SDL_HITTEST_NORMAL:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
            break;
        case SDL_HITTEST_DRAGGABLE:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
            break;
        case SDL_HITTEST_RESIZE_TOPLEFT:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_NW_RESIZE);
            break;
        case SDL_HITTEST_RESIZE_TOP:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_N_RESIZE);
            break;
        case SDL_HITTEST_RESIZE_TOPRIGHT:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_NE_RESIZE);
            break;
        case SDL_HITTEST_RESIZE_RIGHT:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_E_RESIZE);
            break;
        case SDL_HITTEST_RESIZE_BOTTOMRIGHT:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_SE_RESIZE);
            break;
        case SDL_HITTEST_RESIZE_BOTTOM:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_S_RESIZE);
            break;
        case SDL_HITTEST_RESIZE_BOTTOMLEFT:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_SW_RESIZE);
            break;
        case SDL_HITTEST_RESIZE_LEFT:
            sys_cursors[r] = Wayland_CreateSystemCursor(SDL_SYSTEM_CURSOR_W_RESIZE);
            break;
        }
        r++;
    }

#ifdef SDL_USE_LIBDBUS
    Wayland_DBusInitCursorProperties(d);
#endif

    SDL_SetDefaultCursor(Wayland_CreateDefaultCursor());
}

void Wayland_FiniMouse(SDL_VideoData *data)
{
    Wayland_FreeCursorThemes(data);

#ifdef SDL_USE_LIBDBUS
    Wayland_DBusFinishCursorProperties();
#endif

    for (int i = 0; i < SDL_arraysize(sys_cursors); i++) {
        Wayland_FreeCursor(sys_cursors[i]);
        sys_cursors[i] = NULL;
    }
}

void Wayland_SetHitTestCursor(SDL_HitTestResult rc)
{
    if (rc == SDL_HITTEST_NORMAL || rc == SDL_HITTEST_DRAGGABLE) {
        SDL_SetCursor(NULL);
    } else {
        Wayland_ShowCursor(sys_cursors[rc]);
    }
}

#endif // SDL_VIDEO_DRIVER_WAYLAND
