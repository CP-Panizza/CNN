#ifndef _POOLING_HPP_
#define _POOLING_HPP_



#include "matrix.hpp"
#include "utils.h"
#include <vector>


class Pooling {
public:
    int pool_size;
    int stride;

    std::vector<std::vector<Matrix<double> *>> *mask = nullptr;

    std::vector<int> out_shape; //输出的维度信息 N,C,H,W

    std::vector<int> input_shape;

    Pooling(int pool_size, int stride, std::vector<int> &input_shape) : pool_size(pool_size), stride(stride) {
        this->input_shape = input_shape;
        int N = input_shape[0], C = input_shape[1], H = input_shape[2], W = input_shape[3];
        int out_h = 1 + (H - this->pool_size) / this->stride;
        int out_w = 1 + (W - this->pool_size) / this->stride;
        out_shape.push_back(N);
        out_shape.push_back(C);
        out_shape.push_back(out_h);
        out_shape.push_back(out_w);
    }

    std::vector<std::vector<Matrix<double> *> > *forward(std::vector<std::vector<Matrix<double> *> > *x) {
        int N = static_cast<int>(x->size()), C = static_cast<int>(((*x)[0]).size()), H = ((*x)[0])[0]->height, W = ((*x)[0])[0]->width;
        int out_h = 1 + (H - this->pool_size) / this->stride;
        int out_w = 1 + (W - this->pool_size) / this->stride;

        //回收上次循环的内存
        if (this->mask != nullptr) {
            free_data(this->mask);
            this->mask = nullptr;
        }

        //初始化输出和mask标记
        this->mask = new std::vector<std::vector<Matrix<double> *>>;
        auto out = new std::vector<std::vector<Matrix<double> *>>;
        for (int i = 0; i < N; ++i) {
            this->mask->push_back(std::vector<Matrix<double> *>(C));
            out->push_back(std::vector<Matrix<double> *>(C));
        }


        Matrix<double> *mat = nullptr;
        Matrix<double> *each_mask = nullptr;
        Matrix<double> *sub_mat = nullptr;
        double *line = nullptr;
        double max_val = 0;
        int max_idx = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                mat = new Matrix<double>(out_h, out_w);
                each_mask = new Matrix<double>(out_h, out_w);
                for (int h = 0, m = 0; h <= H - this->pool_size; h += this->stride, ++m) {
                    for (int w = 0, n = 0; w <= W - this->pool_size; w += this->stride, ++n) {
                        sub_mat = (*x)[i][j]->SubMat(h, w, this->pool_size, this->pool_size);
                        line = sub_mat->to_line();
                        max_val = max(line, this->pool_size * this->pool_size);
                        max_idx = max_index(line, this->pool_size * this->pool_size);
                        mat->Set(m, n, max_val);
                        each_mask->Set(m, n, max_idx);
                        delete (sub_mat);
                        delete[](line);
                    }
                }
                (*mask)[i][j] = each_mask;
                (*out)[i][j] = mat;
            }
        }

        return out;
    }

    std::vector<std::vector<Matrix<double> *> > *backword(std::vector<std::vector<Matrix<double> *> > *dout) {
        //初始化out
        int N = this->input_shape[0], C = this->input_shape[1], H = this->input_shape[2], W = this->input_shape[3];
        auto out = new std::vector<std::vector<Matrix<double> *>>;
        for (int i = 0; i < N; ++i) {
            out->push_back(std::vector<Matrix<double> *>(C));
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                (*out)[i][j] = new Matrix<double>(H, W);
            }
        }

        //计算pooling反向传播
        Matrix<double> *_mask = nullptr;
        int idx;
        double val;
        int no;
        for (int i = 0; i < this->mask->size(); ++i) {
            for (int j = 0; j < (*this->mask)[i].size(); ++j) {
                _mask = (*this->mask)[i][j];
                for (int m = 0; m < _mask->height; m+=this->pool_size) {
                    for (int n = 0; n < _mask->width; n+=this->pool_size) {
                        idx = static_cast<int>(_mask->Get(m, n));
                        val = (*dout)[i][j]->Get(m, n);
                        no = 0;
                        for (int k = m * this->stride; k < (m * this->stride) + this->pool_size; ++k) {
                            for (int p = n * this->stride; p < (n * this->stride) + this->pool_size; ++p) {
                                if (no++ == idx) {
                                    (*out)[i][j]->Set(k, p, val);
                                    goto done;
                                }
                            }
                        }
                        done:;
                    }
                }
            }
        }

        return out;
    }
};

#endif //_POOLING_HPP_