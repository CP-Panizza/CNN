cmake_minimum_required(VERSION 3.12)
project(Convelution)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        Convelution
        main.cpp
        load_data.hpp
        matrix.hpp
        utils.cpp
        utils.h
        convolution.hpp
        poling.hpp
        )