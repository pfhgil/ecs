cmake_minimum_required(VERSION 3.29)
project(ecs)

set(CMAKE_CXX_STANDARD 23)

add_executable(ecs main.cpp components_allocator.cpp components_allocator.h traits.h)
