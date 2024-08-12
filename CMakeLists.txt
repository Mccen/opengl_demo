cmake_minimum_required(VERSION 3.10)
project(demo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILECOMMANDS ON)
set(R ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${R}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${R}/lib/output)
file(GLOB_RECURSE RESOURCE_FILES "resources/**/*")
install(FILES ${RESOURCE_FILES}
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
if (UNIX AND NOT APPLE) # 对于 Linux 和类 Unix 系统
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
elseif (APPLE) # 对于 macOS
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
elseif (WIN32) # 对于 Windows
    find_program(MINGW_GCC_EXECUTABLE gcc)
    find_program(MINGW_GPP_EXECUTABLE g++)
    if (MINGW_GCC_EXECUTABLE AND MINGW_GPP_EXECUTABLE)
        set(CMAKE_C_COMPILER "${MINGW_GCC_EXECUTABLE}")
        set(CMAKE_CXX_COMPILER "${MINGW_GPP_EXECUTABLE}")
    endif()
endif()

include_directories(${R}/include)
add_subdirectory(src)
add_subdirectory(lib)

add_executable(demo ${SRC})
target_link_libraries(demo soil2)
target_link_libraries(demo glfw)
target_link_libraries(demo glad)