cmake_minimum_required(VERSION 3.15)
project(raytracer LANGUAGES CXX)

# C++17, strictly required, no compiler extensions (e.g. no -std=gnu++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Debug if no build type specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(raytracer
    src/raytracer.cpp
)

# Tell the compiler where to find headers (include/ folder at project root)
target_include_directories(raytracer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Per-config warning flags
target_compile_options(raytracer PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)