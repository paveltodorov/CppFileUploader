cmake_minimum_required(VERSION 3.14)
project(file_uploader)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES src/main.cpp)

add_executable(file_uploader ${SOURCES})
