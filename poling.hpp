#ifndef _POOLING_HPP_
#define _POOLING_HPP_

#include "matrix.h"
#include <vector>
#include "utils.h"
class Pooling {
public:
    int int pool_h;
    int pool_w;
    int stride;
    int pad;

    Pooling(int _pool_h, int _pool_w, int _stride = 1, int _pad = 0) :
            pool_h(_pool_h),
            pool_w(_pool_w),
            stride(_stride),
            pad(_pad) {
    }

    std::vector<std::vector<Matrix *>> *forward(std::vector<std::vector<Matrix *>> *x) {
        int N = static_cast<int>(x->size()), C = static_cast<int>(((*x)[0]).size()), H = ((*x)[0])[0]->height, W = ((*x)[0])[0]->width;
        Matrix * img_col = im2col(x, this->pool_h, this->pool_w, this->stride, sthis->pad);
        Matrix *col = img_col->Reshape(-1, this->pool_w * this->pool_h);

    }
};


#endif //_POOLING_HPP_