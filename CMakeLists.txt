cmake_minimum_required(VERSION 3.11)
project(Cnn)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        Cnn
        main.cpp
        load_data.hpp
        matrix.hpp
        utils.cpp
        utils.h
        conv.hpp
        conv_relu.hpp
        pooling.hpp
        pooling_affine.hpp
        relu.hpp
        affine.hpp
        dropout.hpp
        conv_dropout.hpp
        deep_cnn.hpp
        sigmoid.hpp
        softmax.hpp
        cnn.hpp
        adam.hpp
        pgmer.hpp
)