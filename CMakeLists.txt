cmake_minimum_required(VERSION 3.14)
project(ecc)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)

add_executable(ecc main.cpp curve/LongInt.cpp curve/LongInt.h curve/EllipticCurve.cpp curve/EllipticCurve.h curve/Point.cpp curve/Point.h
        dependences/picosha2.h dependences/json.hpp curve/Curve_parameters.cpp curve/Curve_parameters.h curve/ECC.cpp curve/ECC.h
        dependences/AES.h dependences/AES.cpp)

include_directories(usr/local/include)
link_directories(usr/local/lib)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} gmp)
