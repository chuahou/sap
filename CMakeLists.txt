# basic configuration
cmake_minimum_required(VERSION 3.10)
project(sap VERSION 0.0.1 DESCRIPTION "A simple argument parser")

# directories
include_directories(include)
file(GLOB header include/*.h)
file(GLOB src src/*.c src/*.cpp)

# target
add_library(sap STATIC ${src})

# installation
set(include_dest "include/sap")
set(main_lib_dest "lib/sap")
install(TARGETS sap DESTINATION "${main_lib_dest}")
install(FILES ${header} DESTINATION "${include_dest}")

# tests
add_subdirectory(tests)
enable_testing()
add_test(NAME ctests COMMAND ctests)
add_test(NAME cpptests COMMAND cpptests)
