cmake_minimum_required(VERSION 3.24)
project(Final_Project)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE CPP_FILES "src/*.cpp")

include_directories(include/)

add_executable(Final_Project ${CPP_FILES})
