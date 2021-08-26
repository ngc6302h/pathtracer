cmake_minimum_required(VERSION 3.18)
project(pathtracer)

set(CMAKE_CXX_STANDARD 20)

add_executable(pathtracer main.cpp)

target_include_directories(pathtracer PRIVATE neomath)
target_include_directories(pathtracer PRIVATE neo)

target_link_libraries(pathtracer SDL2)
