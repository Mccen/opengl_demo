find_package(OpenGL REQUIRED)
set_source_files_properties(*.c PROPERTIES LANGUAGE C)
set_source_files_properties(*.cpp PROPERTIES LANGUAGE CXX)
# SOIL2
set(SOIL2_BUILD_TESTS OFF)
set(SOIL2_SKIP_HEADERS OFF)
add_subdirectory(SOIL2)
# glfw

set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
# glad
add_library(glad STATIC glad/glad.c)