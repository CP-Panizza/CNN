#ifndef _CONV_HPP_
#define _CONV_HPP_

#include "matrix.hpp"
#include <vector>
#include "utils.h"

class Conv {
public:
    int _img_w;
    int _img_h;
    int _filter_num;
    int _filter_size;  //卷积核边长
    int _channel;
    int _batch_size;
    int _stride;
    int _pad;

    Matrix<double> *W = nullptr;
    Matrix<double> *b = nullptr;
    Matrix<double> *col = nullptr;

    Matrix<double> *dW = nullptr;
    Matrix<double> *db = nullptr;

    std::vector<int> out_shape; //输出的维度信息 N,C,H,W

    Conv(double std_init_whight, int _filter_num, int _filter_size,
         int _stride, int _pad, const std::vector<int> &input_shape) :
                                  _filter_num(_filter_num),
                                  _filter_size(_filter_size),
                                  _stride(_stride),
                                  _pad(_pad) {
        this->_batch_size = input_shape[0];
        this->_channel = input_shape[1];
        this->_img_h = input_shape[2];
        this->_img_w = input_shape[3];
        auto w = rand_matrix(this->_channel * _filter_size * _filter_size, _filter_num);
        this->W = w->operator*(std_init_whight);
//        std::cout << "conv_w:\n" << this->W;
        delete (w);
        auto _b = rand_matrix(1, _filter_num);
        this->b = _b->operator*(0);
//        std::cout << "conv_b:\n" << this->b;
        delete (_b);
        int out_h = (_img_h + 2 * this->_pad - this->_filter_size) / this->_stride + 1;
        int out_w = (_img_w + 2 * this->_pad - this->_filter_size) / this->_stride + 1;

        out_shape.push_back(_batch_size);
        out_shape.push_back(_filter_num);
        out_shape.push_back(out_h);
        out_shape.push_back(out_w);
    }

    std::vector<std::vector<Matrix<double> *>> *forward(std::vector<std::vector<Matrix<double> *> > *x) {
        int N = static_cast<int>(x->size()), C = static_cast<int>(((*x)[0]).size()), H = ((*x)[0])[0]->height, W = ((*x)[0])[0]->width;
        int out_h = (H + 2 * this->_pad - this->_filter_size) / this->_stride + 1;
        int out_w = (W + 2 * this->_pad - this->_filter_size) / this->_stride + 1;
        if (this->col != nullptr) {
            delete (this->col);
        }

        this->col = im2col(x, this->_filter_size, this->_filter_size, this->_stride, this->_pad);
        auto col_dot_w = this->col->Dot(this->W);
        auto col_dot_w_add_b = col_dot_w->operator+(this->b);
        delete (col_dot_w);


        int conved_img_size = out_h * out_w; //卷积后得到的一张图片的size
        int num = col_dot_w_add_b->height / conved_img_size;
        int _channel = col_dot_w_add_b->width;
        auto out = new std::vector<std::vector<Matrix<double> *>>;
        for (int i = 0; i < num; ++i) {
            out->push_back(std::vector<Matrix<double> *>(_channel));
        }

        Matrix<double> *each_col = nullptr;
        Matrix<double> *each_col_reshape = nullptr;
        for (int i = 0; i < num; ++i) {
            for (int j = 0; j < _channel; ++j) {
                each_col = col_dot_w_add_b->SubMat(i * conved_img_size, j, 1, conved_img_size);
                each_col_reshape = each_col->Reshape(out_h, out_w);
                (*out)[i][j] = each_col_reshape->Copy();
                delete (each_col);
                delete (each_col_reshape);
            }
        }
        delete (col_dot_w_add_b);

        return out;
    }

    std::vector<std::vector<Matrix<double> *> > *backword(std::vector<std::vector<Matrix<double> *>> *dout) {
        //把dout转化成与col_dot_w矩阵相同的矩阵
        int N = static_cast<int>(dout->size()), C = static_cast<int>(((*dout)[0]).size()), H = ((*dout)[0])[0]->height, W = ((*dout)[0])[0]->width;
        int height = N * H * W;
        int width = C;
        auto dout_mat = new Matrix<double>(height, width);
        int img_size = H * W;
        double *a_col = nullptr;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < C; ++j) {
                a_col = (*dout)[i][j]->to_line();
                for (int k = 0; k < img_size; ++k) {
                    dout_mat->Set(i * img_size + k, j, a_col[k]);
                }
                delete[](a_col);
            }
        }

        auto w_t = this->W->T();
        auto dx = dout_mat->Dot(w_t);
        if (this->dW != nullptr) {
            delete (this->dW);
        }
        auto x_t = this->col->T();
        this->dW = x_t->Dot(dout_mat);
        if (this->db != nullptr) {
            delete (this->db);
        }
        this->db = sum(dout_mat, "c");
        delete (dout_mat);
        delete (x_t);
        delete(w_t);

        //给原始输入图片进行padding后，im2col之后的每张图片高宽
        int h = 1 + (this->_img_h + (2 * this->_pad) - this->_filter_size) / this->_stride;
        int w = 1 + (this->_img_w + (2 * this->_pad) - this->_filter_size) / this->_stride;

        int each_width = this->_filter_size * this->_filter_size; //子矩阵宽度
        int each_height = dx->height / this->_batch_size;        //子矩阵高度


        int CHANNEL = dx->width / each_width;

        auto out = new std::vector<std::vector<Matrix<double> *>>;
        //截取子矩阵，并还原此子矩阵，然后把还原后的子矩阵去除padding
        for (int i = 0; i < this->_batch_size; ++i) {
            out->push_back(std::vector<Matrix<double> *>(CHANNEL));
        }

        //输入的图片进行pad之后的宽高
        int pad_img_w = this->_img_w + 2 * this->_pad;
        int pad_img_h = this->_img_h + 2 * this->_pad;

        Matrix<double> *each_mat = nullptr;
        Matrix<double> *col_mat = nullptr;
        for (int i = 0; i < this->_batch_size; ++i) {
            for (int j = 0; j < CHANNEL; ++j) {
                each_mat = dx->SubMat(i * each_height, j * each_width, each_width, each_height);
                col_mat = col2im(each_mat, pad_img_w, pad_img_h, this->_filter_size, this->_stride);
                (*out)[i][j] = col_mat->SubMat(this->_pad, this->_pad, this->_img_w, this->_img_h); //去除padding
                delete(each_mat);
                delete(col_mat);
            }
        }

        delete(dx);
        return out;
    }
};


#endif //_CONV_HPP_