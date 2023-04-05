cmake_minimum_required(VERSION 3.24)
project(leo_json)

set(CMAKE_CXX_STANDARD 17)

add_library(json STATIC ./json/json.cpp ./json/json.h)

add_executable(json_test main.cpp)
target_link_libraries(json_test json)
