cmake_minimum_required(VERSION 3.12)
project(Convelution)

set(CMAKE_CXX_STANDARD 11)

add_executable(Convelution
        main.cpp
        matrix.h
        matrix.cpp
        load_data.hpp
        utils.cpp
        utils.h
        convolution.hpp
        )