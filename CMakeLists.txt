cmake_minimum_required(VERSION 3.14)
project(prepdisk CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(prepdisk
        src/main.cpp src/util.cpp src/boot.cpp src/data.cpp)
