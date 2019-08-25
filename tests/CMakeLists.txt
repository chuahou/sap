add_executable(ctests src/ctests.c)
add_executable(cpptests src/cpptests.cpp)
target_link_libraries(ctests sap)
target_link_libraries(cpptests sap)
