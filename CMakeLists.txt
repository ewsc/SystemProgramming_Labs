cmake_minimum_required(VERSION 3.26)
project(SysP)

set(CMAKE_CXX_STANDARD 14)

include_directories(Lab1)
include_directories(Lab2)

add_executable(SysP
        Lab1/main.cpp
        Lab1/main.h
        Lab2/main.cpp)
