# basic configuration
cmake_minimum_required(VERSION 3.10)
project(sap_example VERSION 0.0.1 DESCRIPTION "Example application for SAP")

# target
add_executable(example src/example.cpp)

# header file
target_include_directories(example PRIVATE ../include)
