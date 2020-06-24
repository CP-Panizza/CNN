#ifndef _RELU_HPP_
#define _RELU_HPP_
//pooling_affine之后的relu层

#include "matrix.hpp"

class Relu{
public:
    Matrix<double> *mask = nullptr;
    Matrix<double> *forward(Matrix<double> *x){
        if(this->mask != nullptr){
            delete(this->mask);
        }
        auto out = x->Copy();
        this->mask = x->Copy();
        for (int i = 0; i < this->mask->height; ++i) {
            for (int j = 0; j < this->mask->width; ++j) {
                if(this->mask->Get(i, j) <= 0){
                    this->mask->Set(i, j, 1);
                    out->Set(i, j, 0);
                } else {
                    this->mask->Set(i, j, 0);
                }
            }
        }
        return out;
    }

    Matrix<double> * backward(Matrix<double> *dout){
        auto temp = dout->Copy();
        for (int i = 0; i < this->mask->height; ++i) {
            for (int j = 0; j < this->mask->width; ++j) {
                if(this->mask->Get(i, j) == 1){
                    temp->Set(i, j, 0);
                }
            }
        }
        return temp;
    }
};

#endif //_RELU_HPP_