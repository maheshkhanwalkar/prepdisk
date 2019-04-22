cmake_minimum_required(VERSION 3.14)
project(prepdisk CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(prepdisk src/main.cpp src/util.cpp src/util.hpp src/boot.cpp src/boot.hpp src/data.cpp src/data.hpp)