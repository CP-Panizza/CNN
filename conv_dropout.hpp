#ifndef _CONV_DROPOUT_HPP_
#define _CONV_DROPOUT_HPP_

#include "matrix.hpp"
#include "utils.h"
#include <vector>
/**
 * 处理四维数据的dropout
 */
class ConvDropout {
public:
    std::vector<std::vector<Matrix<double>*> > *mask = nullptr;
    bool is_train = true; //训练标记
    double dropout_ratio = 0.5;

    explicit ConvDropout(double _dropout_ratio = 0.5){
        this->dropout_ratio = _dropout_ratio;
    }

    std::vector<std::vector<Matrix<double> *> > *
    forward(std::vector<std::vector<Matrix<double>*> > *x) {
        int N = static_cast<int>(x->size()), C = static_cast<int>(((*x)[0]).size()), H = ((*x)[0])[0]->height, W = ((*x)[0])[0]->width;
        if(this->is_train){
            if(this->mask != nullptr){
                free_data(this->mask);
            }

            auto out = new std::vector<std::vector<Matrix<double>*> >;
            this->mask = new std::vector<std::vector<Matrix<double>*> >;
            for (int k = 0; k < N; ++k) {
                this->mask->push_back(std::vector<Matrix<double>*>(C));
            }
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < C; ++j) {
                    (*this->mask)[i][j] = rand_matrix(H, W);
                }
                out->push_back(std::vector<Matrix<double>*>(C));
            }

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < C; ++j) {
                    for (int m = 0; m < H; ++m) {
                        for (int n = 0; n < W; ++n) {
                            if((*this->mask)[i][j]->Get(m,n) > this->dropout_ratio){
                                (*this->mask)[i][j]->Set(m,n, 1);
                            } else {
                                (*this->mask)[i][j]->Set(m,n, 0);
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < C; ++j) {
                    (*out)[i][j] = (*x)[i][j]->operator*((*this->mask)[i][j]);
                }
            }
            return out;
        } else {
            auto out = new std::vector<std::vector<Matrix<double>*> >;
            for (int i = 0; i < N; ++i) {
                out->push_back(std::vector<Matrix<double>*>(C));
            }

            double val = 1.0 - this->dropout_ratio;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < C; ++j) {
                    (*out)[i][j] = (*x)[i][j]->operator*(val);
                }
            }
            return out;
        }
    }

    std::vector<std::vector<Matrix<double> *> > *
    backward(std::vector<std::vector<Matrix<double>*> > *dout) {
        int N = static_cast<int>(dout->size()), C = static_cast<int>(((*dout)[0]).size()), H = ((*dout)[0])[0]->height, W = ((*dout)[0])[0]->width;
        auto dx = new std::vector<std::vector<Matrix<double>*> >;
        for (int i = 0; i < N; ++i) {
            dx->push_back(std::vector<Matrix<double>*>(C));
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                (*dx)[i][j] = (*dout)[i][j]->operator*((*this->mask)[i][j]);
            }
        }

        return dx;
    }
};


#endif //_CONV_DROPOUT_HPP_