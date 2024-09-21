find_package(OpenGL REQUIRED)
set_source_files_properties(*.c PROPERTIES LANGUAGE C)
set_source_files_properties(*.cpp PROPERTIES LANGUAGE CXX)

# # glfw
# set(BUILD_SHARED_LIBS ON)
# set(GLFW_BUILD_EXAMPLES OFF)
# set(GLFW_BUILD_TESTS OFF)
# set(GLFW_BUILD_DOCS OFF)
# set(GLFW_INSTALL OFF)
# add_subdirectory(glfw)

# glad
include_directories(glad/include)
#add_library(glad STATIC glad/src/gl.c)
add_library(glad SHARED glad/src/gl.c)

# stb_image
set(GENERATE_C "//启用stb_image\n#define STB_IMAGE_IMPLEMENTATION\n#include \"stb_image.h\"")
file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.c" "${GENERATE_C}")
#add_library(stb STATIC stb/stb_image.c)
add_library(stb SHARED stb/stb_image.c)

# glm
set(GLM_TEST OFF)
set(GLM_BUILD_LIBRARY ON)
set(GLM_BUILD_INSTALL OFF)
set(GLM_BUILD_LIBRARY OFF)
set(GLM_ENABLE_CXX_17 ON)
set(BUILD_SHARED_LIBS ON)
add_subdirectory(glm)

# sdl
set(BUILD_SHARED_LIBS ON)
set(SDL_SHARED ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_EXAMPLES OFF)
set(HAVE_TESTS OFF)
add_subdirectory(sdl)