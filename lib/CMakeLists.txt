# project(soil2 LANGUAGES CXX C VERSION 1.0.0)
find_package(OpenGL REQUIRED)
set_source_files_properties(*.c PROPERTIES LANGUAGE C)
set_source_files_properties(*.cpp PROPERTIES LANGUAGE CXX)
# SOIL2
file(GLOB soil SOIL2/*.c SOIL2/*.h SOIL2/*.cpp)
add_library(soil2 STATIC  ${soil})
target_link_libraries(soil2 PRIVATE OpenGL::GL)

# glfw
file(GLOB glfw  GLFW/glfw3.h
    GLFW/glfw3native.h
    glfw/internal.h glfw/platform.h glfw/mappings.h
    glfw/context.c glfw/init.c glfw/input.c glfw/monitor.c glfw/platform.c glfw/vulkan.c glfw/window.c
    glfw/egl_context.c glfw/osmesa_context.c glfw/null_platform.h glfw/null_joystick.h
    glfw/null_init.c glfw/null_monitor.c glfw/null_window.c glfw/null_joystick.c)
add_library(glfw STATIC  ${glfw})

set(libos glfw/win32_time.h glfw/win32_thread.h
    glfw/win32_module.c glfw/win32_time.c glfw/win32_thread.c)

target_sources(glfw PRIVATE ${libos})
target_compile_definitions(glfw PRIVATE _GLFW_WIN32)
target_sources(glfw PRIVATE glfw/win32_platform.h glfw/win32_joystick.h glfw/win32_init.c
                                glfw/win32_joystick.c glfw/win32_monitor.c glfw/win32_window.c
                                glfw/wgl_context.c)


# glad
add_library(glad STATIC glad/glad.c)