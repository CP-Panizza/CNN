//
// Created by Administrator on 2020/6/4.
//

#ifndef CONVELUTION_UTILS_H
#define CONVELUTION_UTILS_H

#include <vector>
#include "matrix.h"
Matrix *im2col(std::vector<std::vector<Matrix *>> *input_data, int filter_h, int filter_w, int stride=1, int pad=0);
#endif //CONVELUTION_UTILS_H
