cmake_minimum_required(VERSION 3.14)
project(ecc)

set(CMAKE_CXX_STANDARD 17)

add_executable(ecc main.cpp LongInt.cpp LongInt.h)