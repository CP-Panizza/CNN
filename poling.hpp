#ifndef _POOLING_HPP_
#define _POOLING_HPP_

#include "matrix.hpp"
#include <vector>
#include "utils.h"
class Pooling {
public:
    int pool_h;
    int pool_w;
    int stride;
    int pad;
    std::vector<std::vector<Matrix<double> *>> *x;
    Matrix<int> *arg_max;

    Pooling(int _pool_h, int _pool_w, int _stride = 1, int _pad = 0) :
            pool_h(_pool_h),
            pool_w(_pool_w),
            stride(_stride),
            pad(_pad) {
    }

    std::vector<std::vector<Matrix<double> *> > *forward(std::vector<std::vector<Matrix<double> *> > *_x) {
        int N = static_cast<int>(_x->size()), C = static_cast<int>(((*_x)[0]).size()), H = ((*_x)[0])[0]->height, W = ((*_x)[0])[0]->width;
        int out_h = (H - this->pool_h) / this->stride + 1;
        int out_w = (W - this->pool_w) / this->stride + 1;
        Matrix<double> * img_col = im2col(_x, this->pool_h, this->pool_w, this->stride, this->pad);
        Matrix<double> *col = img_col->Reshape(-1, this->pool_w * this->pool_h);

        auto arg_max_ = argmax(col, "r");
        auto max_ = max(col, "r");
        auto t = max_->Reshape(-1, C);

        std::vector<Matrix<double> *> *reshape = new std::vector<Matrix<double> *>;
        int cut =  t->height / N;
        for (int i = 0; i < t->height; i += cut) {
            auto data = Create2dArray<double>(cut, t->width);
            for (int j = 0; j < cut; ++j) {
                auto m = t->Row(i + j);
                for (int k = 0; k < t->width; ++k) {
                    data[j][k] = m->Get(0, k);
                }
                delete (m);
            }
            reshape->push_back(new Matrix<double>(cut, t->width, data));
        }

        std::vector<std::vector<Matrix<double> *> > * out = new std::vector<std::vector<Matrix<double> *> >;
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
                auto xx = new Matrix<double>(h, w, data);
                std::cout <<"\n" << xx;
                conv_imgs->push_back(xx);
            }
            out->push_back(*conv_imgs);
        }
        this->x = _x;
        this->arg_max = arg_max_;
        return out;
    }
};


#endif //_POOLING_HPP_