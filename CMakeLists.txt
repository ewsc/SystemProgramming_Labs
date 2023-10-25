cmake_minimum_required(VERSION 3.26)
project(SysP)

set(CMAKE_CXX_STANDARD 14)

include_directories(L1)
include_directories(L2)

add_executable(SysP
        L1/main.cpp
        L1/main.h
        L2/main.cpp)
