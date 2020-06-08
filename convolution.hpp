#ifndef _CONVELUTION_HPP_
#define _CONVELUTION_HPP_

#include <vector>
#include <iostream>
#include "matrix.hpp"
#include "utils.h"

class Convelution {
public:
    std::vector<Matrix<double> *> *W;
    std::vector <std::vector<Matrix<double> *>> *x;
    Matrix<double> *col;
    Matrix<double> *col_W;
    Matrix<double> *b;
    int filter_w;
    int filter_h;

    Convelution(std::vector<Matrix<double> *> *_W, Matrix<double> *_b){
        this->W = _W;
        this->b = _b;
        this->filter_h = (*_W->begin())->height;
        this->filter_w =  (*_W->begin())->width;
    }

    Matrix<double> *W2FilterMtarix(){
        int h = this->filter_h*this->filter_w;
        int w = static_cast<int>(this->W->size());
        auto data = Create2dArray<double>(h, w);
        int col = 0, row = 0;
        for(auto &f : *this->W){
            for (int i = 0; i < f->height; ++i) {
                auto line = f->Row(i);
                for (int j = 0; j < line->width; ++j) {
                    data[row++][col] = line->Get(0, j);
                }
            }
            col++;
            row = 0;
        }
        return new Matrix<double>(h, w, data);
    }

    std::vector<std::vector<Matrix<double> *> > * forward(std::vector <std::vector<Matrix<double> *>> *imgs) {
        int N = static_cast<int>(imgs->size()), C = static_cast<int>(((*imgs)[0]).size()), H = ((*imgs)[0])[0]->height, W = ((*imgs)[0])[0]->width;
        int pad = 0;
        int stride = 1;
        int out_h = (H + 2 * 0 - this->filter_h) / stride + 1;
        int out_w = (W + 2 * 0 - this->filter_w) / stride + 1;

        Matrix<double> *col_ = im2col(imgs, this->filter_h, this->filter_w);
        Matrix<double> *col_W_ = this->W2FilterMtarix();
//        std::cout<<"col:\n" << col_;
        Matrix<double> *out = (col_->Dot(col_W_))->operator+(this->b);
//        std::cout << "out:\n"<< out;
        std::vector<Matrix<double> *> *reshape = new std::vector<Matrix<double> *>;


        int cut = out->height / N;
        for (int i = 0; i < out->height; i += cut) {
            auto data = Create2dArray<double>(cut, out->width);
            for (int j = 0; j < cut; ++j) {
                auto m = out->Row(i + j);
                for (int k = 0; k < out->width; ++k) {
                    data[j][k] = m->Get(0, k);
                }
                delete (m);
            }
            reshape->push_back(new Matrix<double>(cut, out->width, data));
        }

        std::vector<std::vector<Matrix<double> *> > * convs = new std::vector<std::vector<Matrix<double> *> >;
        for (auto &x:*reshape) {
            auto *conv_imgs = new std::vector<Matrix<double> *>;
            for (int i = 0; i < x->width; ++i) {
                auto temp = x->Col(i);
                auto _col = temp->T();
                delete (temp);
                int w = out_w;
                int h = _col->width / w;
                auto data = Create2dArray<double>(h, w);
                int index = 0;
                for (int m = 0; m < h; ++m) {
                    for (int n = 0; n < w; ++n) {
                        data[m][n] = _col->Get(0, index++);
                    }
                }
                delete(_col);
                conv_imgs->push_back(new Matrix<double>(h, w, data));
            }
            convs->push_back(*conv_imgs);
        }
        this->x = imgs;
        this->col = col_->Copy();
        this->col_W = col_W_->Copy();
        delete(col_);
        delete(col_W_);
        return convs;
    }
};


#endif //_CONVELUTION_HPP_