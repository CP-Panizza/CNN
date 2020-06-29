#ifndef _POOLING_AFFINE_HPP_
#define _POOLING_AFFINE_HPP_



#include "matrix.hpp"
#include "utils.h"

class PoolingAffine {
public:
    Matrix<double> *W = nullptr;
    Matrix<double> *b = nullptr;
    Matrix<double> *X = nullptr;

    Matrix<double> *dW = nullptr;
    Matrix<double> *db = nullptr;

    std::vector<int> out_shape; //h, w
    std::vector<int> input_shape;

    PoolingAffine(double std_init_whight, std::vector<int> &input_shape, int out_size) {
        this->input_shape = input_shape;
        int N = input_shape[0], C = input_shape[1], H = input_shape[2], W = input_shape[3];
        int width = C * H * W;
        auto rand_mat = rand_matrix(width, out_size);
        this->W = rand_mat->operator*(std_init_whight);
        auto r_b = rand_matrix(1, out_size);
        this->b = r_b->operator*(0.0);
        delete (rand_mat);
        delete (r_b);
        this->out_shape.push_back(N);
        this->out_shape.push_back(out_size);
    }

    Matrix<double> *forward(std::vector<std::vector<Matrix<double> *> > *x) {
        int N = static_cast<int>(x->size()), C = static_cast<int>(((*x)[0]).size()), H = ((*x)[0])[0]->height, W = ((*x)[0])[0]->width;

        //把所有通道图片展开成一行,转换成二维vector
        std::vector<std::vector<double> > to_mat;
        for (int i = 0; i < N; ++i) {
            std::vector<double> line;
            for (int j = 0; j < C; ++j) {
                for (int m = 0; m < (*x)[i][j]->height; ++m) {
                    for (int n = 0; n < (*x)[i][j]->width; ++n) {
                        line.push_back((*x)[i][j]->Get(m, n));
                    }
                }
            }
            to_mat.push_back(line);
        }

        //二维vector转换成矩阵
        int height = static_cast<int>(to_mat.size());
        int width = static_cast<int>(to_mat[0].size());
        auto mat = new Matrix<double>(height, width);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                mat->Set(i, j, to_mat[i][j]);
            }
        }

        if (this->X != nullptr) {
            delete (this->X);
        }

        this->X = mat->Copy();
        auto x_dot_w = mat->Dot(this->W);
        auto out = x_dot_w->operator+(this->b);
        delete (mat);
        delete (x_dot_w);
        return out;
    }

    std::vector<std::vector<Matrix<double> *> > *backword(Matrix<double> *dout) {
        auto w_t = this->W->T();
        auto dx = dout->Dot(w_t);
        if (this->dW != nullptr) {
            delete (this->dW);
        }
        auto x_t = this->X->T();
        this->dW = x_t->Dot(dout);
        if (this->db != nullptr) {
            delete (this->db);
        }
        this->db = sum(dout, "c");
        delete (w_t);
        delete (x_t);


        //把dx二维矩阵转换成四维数组
        int N = this->input_shape[0], C = this->input_shape[1], H = this->input_shape[2], W = this->input_shape[3];

        auto out = new std::vector<std::vector<Matrix<double> *>>;
        for (int i = 0; i < N; ++i) {
            out->push_back(std::vector<Matrix<double> *>(C));
        }

        int cut = H * W;
        Matrix<double> *line = nullptr;
        Matrix<double> *reshaped_mat = nullptr;
        for (int i = 0; i < dx->height; ++i) {
            for (int j = 0; j < C; ++j) {
                line = dx->SubMat(i, j * cut, cut, 1);
                reshaped_mat = line->Reshape(H, W);
                delete (line);
                (*out)[i][j] = reshaped_mat;
            }
        }
        delete (dx);

        return out;
    }
};

#endif //_POOLING_AFFINE_HPP_