cmake_minimum_required(VERSION 3.10)
project(STLToys)

set(CMAKE_CXX_STANDARD 11)

add_executable(STLToys main.cpp yyalloc.h YY_construct.h YY_alloc.h)