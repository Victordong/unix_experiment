cmake_minimum_required(VERSION 3.10)
project(untitled7)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled7 src/main.cpp src/request.h src/response.h src/parser.h src/work_list.h src/data.h src/parser.cpp src/request.cpp src/data.cpp src/work_list.cpp src/response.cpp)