#include "matrix.h"

Matrix *Zeros(int h, int w) {
    return new Matrix(h, w, Create2dArray<double>(h, w));
}

std::ostream &operator<<(std::ostream &os, Matrix *matrix) {
    for (int i = 0; i < matrix->height; ++i) {
        for (int j = 0; j < matrix->width; ++j) {
            os << matrix->Get(i, j) << ", ";
        }
        os << "\n";
    }
    return os;
}


