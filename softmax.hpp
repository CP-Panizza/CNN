#ifndef _SOFTMAX_HPP_
#define _SOFTMAX_HPP_

#include "matrix.hpp"
#include "utils.h"

Matrix<double> *softmax(Matrix<double> *x){
    auto _x = x->T();
    auto max_col = max(_x, "c");
    auto r = _x->operator-(max_col);
    auto _mat_exp = mat_exp(r);
    auto _sum_mat_exp = sum(_mat_exp, "c");
    auto y = _mat_exp->operator/(_sum_mat_exp);
    auto out = y->T();

    delete(_x);
    delete(max_col);
    delete(r);
    delete(_mat_exp);
    delete(_sum_mat_exp);
    delete(y);
    return out;
}



class Softmax{
public:
    Matrix<double> *t = nullptr;
    Matrix<double> *y = nullptr;
    Softmax(){}
    Matrix<double> * forward(Matrix<double> *_x, Matrix<double> *_t){
        if(this->t != nullptr){
            delete(this->t);
        }
        this->t = _t->Copy();
        if(this->y != nullptr){
            delete(this->y);
        }
        this->y = softmax(_x);
        return this->y->Copy();
    }

    Matrix<double> *backward(){

        auto y_t = this->y->operator-(this->t);
        auto dx = y_t->operator/(this->y->height);

        delete(y_t);
        return dx;
    }
};


#endif //_SOFTMAX_HPP_