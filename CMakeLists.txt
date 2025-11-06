cmake_minimum_required(VERSION 4.0)
project(Raylib-Gremlin C)

set(CMAKE_C_STANDARD 11)
find_package(raylib REQUIRED)
add_executable(Raylib-Gremlin main.c)
target_link_libraries(Raylib-Gremlin PRIVATE raylib m)