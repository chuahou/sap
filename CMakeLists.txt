cmake_minimum_required(VERSION 3.10)
project(sap VERSION 0.0.1 DESCRIPTION "A simple argument parser")
include_directories(include)
file(GLOB src src/*.c src/*.cpp)

add_library(sap STATIC ${src})

add_subdirectory(tests)

enable_testing()
add_test(NAME ctests COMMAND ctests)
add_test(NAME cpptests COMMAND cpptests)