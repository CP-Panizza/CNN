#ifndef _CONVELUTION_HPP_
#define _CONVELUTION_HPP_

#include <vector>
#include <iostream>
#include "matrix.h"
#include "utils.h"

class Convelution {
public:
    std::vector<Matrix *> *W;
    std::vector <std::vector<Matrix *>> *x;
    Matrix *col;
    Matrix *col_W;
    Matrix *b;
    int filter_w;
    int filter_h;

    Convelution(std::vector<Matrix *> *_W, Matrix *_b){
        this->W = _W;
        this->b = _b;
        this->filter_h = (*_W->begin())->height;
        this->filter_w =  (*_W->begin())->width;
    }

    Matrix *W2FilterMtarix(){
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
        return new Matrix(h, w, data);
    }

    std::vector<std::vector<Matrix *> *> * forward(std::vector <std::vector<Matrix *>> *imgs) {
        int N = static_cast<int>(imgs->size()), C = static_cast<int>(((*imgs)[0]).size()), H = ((*imgs)[0])[0]->height, W = ((*imgs)[0])[0]->width;
        int pad = 0;
        int stride = 1;
        int out_h = (H + 2 * 0 - this->filter_h) / stride + 1;
        int out_w = (W + 2 * 0 - this->filter_w) / stride + 1;

        Matrix *col_ = im2col(imgs, this->filter_h, this->filter_w);
        Matrix *col_W_ = this->W2FilterMtarix();
//        std::cout<<"col:\n" << col_;
        Matrix *out = (col_->Dot(col_W_))->operator+(this->b);
//        std::cout << "out:\n"<< out;
        std::vector<Matrix *> *reshape = new std::vector<Matrix *>;

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
            reshape->push_back(new Matrix(cut, out->width, data));
        }

        std::vector<std::vector<Matrix *> *> * convs = new std::vector<std::vector<Matrix *> *>;
        for (auto &x:*reshape) {
            auto *conv_imgs = new std::vector<Matrix *>;
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
                auto xx = new Matrix(h, w, data);
//                std::cout <<"\n" << xx;
                conv_imgs->push_back(xx);
            }
            convs->push_back(conv_imgs);
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