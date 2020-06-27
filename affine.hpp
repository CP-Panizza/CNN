#ifndef _AFFINE_HPP_
#define _AFFINE_HPP_

#include "matrix.hpp"
#include "utils.h"

class Affine {
public:
    Matrix<double> *x = nullptr;
    Matrix<double> *W = nullptr;
    Matrix<double> *b = nullptr;
    Matrix<double> *dW = nullptr;
    Matrix<double> *db = nullptr;

    std::vector<int> out_shape; //h, w
    Affine(std::vector<int> &input_shape, int out_size){
        int height = input_shape[0];
        int width = input_shape[1];
        auto rand_mat = rand_matrix(width, out_size);
        this->W = rand_mat->operator*(0.1);
        auto r_b = rand_matrix(1, out_size);
        this->b = r_b->operator*(0);
        delete(rand_mat);
        delete(r_b);
        this->out_shape.push_back(height);
        this->out_shape.push_back(out_size);
    }

    Matrix<double> *forward(Matrix<double> *_x) {
        if (this->x != nullptr) {
            delete (this->x);
        }
        this->x = _x->Copy();

        auto x_dot_w = this->x->Dot(this->W);
        auto out = x_dot_w->operator+(this->b);
        delete(x_dot_w);
        return out;
    }

    Matrix<double> *backward(Matrix<double> *dout) {
        auto w_t = this->W->T();
        auto dx = dout->Dot(w_t);
        if (this->dW != nullptr) {
            delete (this->dW);
        }
        auto x_t = this->x->T();
        this->dW = x_t->Dot(dout);
        if (this->db != nullptr) {
            delete (this->db);
        }
        this->db = sum(dout, "c");

        delete (w_t);
        delete (x_t);
        return dx;
    }
};

#endif //_AFFINE_HPP_