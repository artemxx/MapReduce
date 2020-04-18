cmake_minimum_required(VERSION 3.16)
project(MapReduce)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled4 mapreduce.cpp)
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)