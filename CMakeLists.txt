cmake_minimum_required(VERSION 3.14)
project(ecc)

set(CMAKE_CXX_STANDARD 17)

add_executable(ecc main.cpp LongInt.cpp LongInt.h EllipticCurve.cpp EllipticCurve.h Point.cpp Point.h ECDSA.cpp ECDSA.h dependences/picosha2.h dependences/json.hpp)

target_link_libraries(${PROJECT_NAME} gmp)
