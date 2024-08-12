cmake_minimum_required(VERSION 3.10)
project(demo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILECOMMANDS ON)
set(R ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${R}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${R}/lib/output)
file(GLOB_RECURSE RESOURCE_FILES "resources/**/*")

include_directories(${R}/include)
add_subdirectory(src)
add_subdirectory(lib)

add_executable(demo ${SRC})
target_link_libraries(demo soil2)
target_link_libraries(demo glfw)
target_link_libraries(demo glad)
install(DIRECTORY resources/
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)