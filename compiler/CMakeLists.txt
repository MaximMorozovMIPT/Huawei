cmake_minimum_required(VERSION 3.16)

# set the project name
project(Compiler)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Compiler main.cpp)
add_executable(DomTree dom_tree_test.cpp)
add_executable(LoopAnalize loop_analizer_test.cpp)