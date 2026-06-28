cmake_minimum_required(VERSION 3.24)

set(CMAKE_SUPPRESS_REGENERATION true)

project(Terra LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/out")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/out")

if(MSVC)
    configure_file(
        "${CMAKE_SOURCE_DIR}/CMake/Directory.Build.targets.in"
        "${CMAKE_BINARY_DIR}/Directory.Build.targets"
        @ONLY
    )
endif()

find_package(OpenGL REQUIRED)

add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/External/OpenGL/GLFW/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/OpenGL/GLFW/include"
)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    "${CMAKE_SOURCE_DIR}/External/OpenGL/glm"
)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE
    "${CMAKE_SOURCE_DIR}/External/stb"
)

add_library(perlin_noise INTERFACE)
target_include_directories(perlin_noise INTERFACE
    "${CMAKE_SOURCE_DIR}/External/PerlinNoise"
)

add_subdirectory(Terra)
add_subdirectory(Game)
add_subdirectory(ScuffedMon)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ScuffedMon)
