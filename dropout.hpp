#ifndef _DROPOUT_HPP_
#define _DROPOUT_HPP_


#include "matrix.hpp"
#include "utils.h"

class Dropout{
public:
    Matrix<double> *mask = nullptr;
    bool is_train = true; //训练标记
    double dropout_ratio = 0.5;

    Matrix<double> *forword(Matrix<double> *x){
        if(this->is_train){
            if(this->mask != nullptr){
                delete(this->mask);
            }
            this->mask = rand_matrix(x->height, x->width);
            for (int i = 0; i < x->height; ++i) {
                for (int j = 0; j < x->width; ++j) {
                    if(this->mask->Get(i,j) > this->dropout_ratio){
                        this->mask->Set(i,j, 1);
                    } else {
                        this->mask->Set(i,j, 0);
                    }
                }
            }
            return x->operator*(this->mask);
        } else {
            return x->operator*(1.0 - this->dropout_ratio);
        }
    }


    Matrix<double> *backward(Matrix<double> *dout){
        return dout->operator*(this->mask);
    }

};


#endif //_DROPOUT_HPP_
