cmake_minimum_required(VERSION 3.22)

project(velocity)

include_directories(headers)

set(CMAKE_CXX_FLAGS "-O2 -Wall")
add_executable(velocity main.cpp)