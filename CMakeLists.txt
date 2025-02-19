cmake_minimum_required(VERSION 3.10)
project(Basic_Raytracer)

set(CMAKE_CXX_STANDARD 17)  # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(raytracer main.cpp)
target_compile_options(raytracer PRIVATE -Wall -Wextra -Werror)
