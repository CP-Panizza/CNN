//
// Created by Administrator on 2020/6/4.
//

#ifndef CONVELUTION_UTILS_H
#define CONVELUTION_UTILS_H

#include <vector>
#include "matrix.hpp"
Matrix<double> *im2col(std::vector<std::vector<Matrix<double> *>> *input_data, int filter_h, int filter_w, int stride=1, int pad=0);
Matrix<int> *argmax(Matrix<double> *mat, std::string dim);
Matrix<double> *max(Matrix<double> *mat, std::string dim);
#endif //CONVELUTION_UTILS_H
