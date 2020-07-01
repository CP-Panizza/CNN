#ifndef _SIGMOID_HPP_
#define _SIGMOID_HPP_


#include "matrix.hpp"
#include "utils.h"


class Sigmoid{
public:
    Matrix<double> *out = nullptr;
    Matrix<double> *forward(Matrix<double> *x){
        auto _x = x->operator*(-1);
        auto _x_exp = mat_exp(_x);
        auto _x_exp_add1 = _x_exp->operator+(1.0);
        for (int i = 0; i < _x_exp_add1->height; ++i) {
            for (int j = 0; j < _x_exp_add1->width; ++j) {
                _x_exp_add1->Set(i,j, 1/_x_exp_add1->Get(i,j));
            }
        }
        if(this->out != nullptr){
            delete(this->out);
        }
        this->out = _x_exp_add1->Copy();
        delete(_x);
        delete(_x_exp);
        return _x_exp_add1;
    }

    Matrix<double> *backward(Matrix<double> *dout){
        auto one_sub_this_out = this->out->Copy();
        for (int i = 0; i < one_sub_this_out->height; ++i) {
            for (int j = 0; j < one_sub_this_out->width; ++j) {
                one_sub_this_out->Set(i,j, 1.0 - one_sub_this_out->Get(i,j));
            }
        }
        auto dout_mul_one_sub_this_out = dout->operator*(one_sub_this_out);
        auto dx = dout_mul_one_sub_this_out->operator*(this->out);
        delete(one_sub_this_out);
        delete(dout_mul_one_sub_this_out);
        return dx;
    }
};

#endif //_SIGMOID_HPP_