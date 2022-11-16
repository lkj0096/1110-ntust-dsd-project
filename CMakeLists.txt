cmake_minimum_required(VERSION 3.18)
project(project03)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SRC_FILES "lib/*.cpp")

add_executable(project03 main.cpp ${SRC_FILES})
