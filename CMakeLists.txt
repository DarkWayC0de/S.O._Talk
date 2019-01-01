cmake_minimum_required(VERSION 3.13)
project(Talk)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -pthread")

find_package(Threads)

add_executable(Talk src/main.cpp src/socket.cpp include/socket.h include/hilo2_enviar.h include/hilo1_recibir.h include/Globasle&Funciones.h)

target_link_libraries(Talk ${CMAKE_THREAD_LIBS_INIT})