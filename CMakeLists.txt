cmake_minimum_required(VERSION 3.28)
project(CompMath)

set(CMAKE_CXX_STANDARD 20)

add_executable(3Polynomial 3_Degree_Polynomial_Solver/main.cpp)
target_compile_options(3Polynomial PRIVATE -O2)

add_executable(3Polynomial_debug 3_Degree_Polynomial_Solver/main.cpp)
target_compile_options(3Polynomial_debug PRIVATE -O0)
