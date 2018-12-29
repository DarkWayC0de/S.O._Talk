cmake_minimum_required(VERSION 3.13)
project(Talk)

set(CMAKE_CXX_STANDARD 14)

add_executable(Talk src/main.cpp src/socket.cpp include/socket.h)