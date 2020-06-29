#ifndef _CONV_RELU_HPP_
#define _CONV_RELU_HPP_



#include "matrix.hpp"
#include <vector>
#include "utils.h"

class ConvRelu{
public:
    std::vector<std::vector< Matrix<double>*> > *mask = nullptr;

    std::vector<std::vector< Matrix<double>*> > *forward(std::vector<std::vector< Matrix<double>*> > *x){
        int N = static_cast<int>(x->size()), C = static_cast<int>(((*x)[0]).size()), H = ((*x)[0])[0]->height, W = ((*x)[0])[0]->width;
        //回收上一次的mask
        if(this->mask != nullptr){
            free_data(this->mask);
            this->mask = nullptr;
        }

        //复制一份和输入一模一样的值作为返回值
        auto out = new std::vector<std::vector< Matrix<double>*>>;
        for (int i = 0; i < N; ++i) {
            out->push_back(std::vector< Matrix<double>*>(C));
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                (*out)[i][j] = (*x)[i][j]->Copy();
            }
        }


        //复制一份和输入一模一样形状的mask标记
        this->mask = new std::vector<std::vector< Matrix<double>*>>;
        for (int i = 0; i < N; ++i) {
            this->mask->push_back(std::vector< Matrix<double>*>(C));
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                (*this->mask)[i][j] = new Matrix<double>(H, W);
            }
        }

        //计算mask,输入中<=0的地方，mask中用1做标记，其余地方用0填充
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                for (int m = 0; m < (*x)[i][j]->height; ++m) {
                    for (int n = 0; n < (*x)[i][j]->width; ++n) {
                        if((*x)[i][j]->Get(m,n) <= 0){
                            (*this->mask)[i][j]->Set(m,n, 1);
                            (*out)[i][j]->Set(m, n, 0);
                        }
                    }
                }
            }
        }

        return out;
    }

    std::vector<std::vector<Matrix<double> *> > *backword(std::vector<std::vector<Matrix<double> *>> *dout) {
        int N = static_cast<int>(dout->size()), C = static_cast<int>(((*dout)[0]).size()), H = ((*dout)[0])[0]->height, W = ((*dout)[0])[0]->width;
        //复制一份和输入一模一样的值作为返回值
        auto out = new std::vector<std::vector< Matrix<double>*>>;
        for (int i = 0; i < N; ++i) {
            out->push_back(std::vector< Matrix<double>*>(C));
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                (*out)[i][j] = (*dout)[i][j]->Copy();
            }
        }

        //mask中值为1的地方对应的out中的位置值设置为0
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                for (int m = 0; m < (*dout)[i][j]->height; ++m) {
                    for (int n = 0; n < (*dout)[i][j]->width; ++n) {
                        if((*this->mask)[i][j]->Get(m,n) == 1){
                            (*out)[i][j]->Set(m, n, 0);
                        }
                    }
                }
            }
        }
        return out;
    }
};


#endif //_CONV_RELU_HPP_