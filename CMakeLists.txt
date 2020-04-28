# basic configuration
cmake_minimum_required(VERSION 3.10)
project(sap VERSION 0.0.2 DESCRIPTION "A simple argument parser")

# directories
file(GLOB header include/*.h)

# tests
add_subdirectory(tests)
enable_testing()
add_test(NAME ctests COMMAND ctests)
add_test(NAME cpptests COMMAND cpptests)
