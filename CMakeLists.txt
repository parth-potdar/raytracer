cmake_minimum_required(VERSION 3.15)
project(raytracer LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable (source lives in the subfolder `raytracer`)
add_executable(raytracer
    raytracer/raytracer.cpp
)

# Enable useful warnings
if (MSVC)
  target_compile_options(raytracer PRIVATE /W4 /permissive-)
else()
  target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic)
endif()
