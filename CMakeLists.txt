cmake_minimum_required(VERSION 3.6)
project(RayTracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Punto.cpp Vector.cpp Matriz.cpp Objetos.cpp RayTracer.cpp)
add_executable(RayTracer ${SOURCE_FILES})