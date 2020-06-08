//
// Created by Administrator on 2020/6/3.
//

#include "utils.h"


Matrix<double> *im2col(std::vector<std::vector<Matrix<double> *>> *input_data, int filter_h, int filter_w, int stride, int pad) {
    int N = static_cast<int>(input_data->size()), C = static_cast<int>(((*input_data)[0]).size()), H = ((*input_data)[0])[0]->height, W = ((*input_data)[0])[0]->width;
    int out_h = (H + 2 * pad - filter_h) / stride + 1;
    int out_w = (W + 2 * pad - filter_w) / stride + 1;
    std::vector<std::vector<double>> result;
    for (auto &vec: *input_data) {
            std::vector<double> line;
            for (int i = 0; i <= H - filter_h; i += stride) {
                for (int j = 0; j <= W - filter_w; j += stride) {
                    for (int n = 0; n < vec.size(); n++) {
                        for (int k = 0; k < filter_h; ++k) {
                            for (int m = 0; m < filter_w; ++m) {
                                line.push_back(vec[n]->Get(i + k, j + m));
                            }
                        }
                    }
                }
            }
            result.push_back(line);
    }

    int width = C * filter_h * filter_w;
    int height = N * (result[0].size() / width);

    double **data = Create2dArray<double>(height, width);

    int row = 0, col = 0;
    int length;
    for (auto &v: result) {
        length = static_cast<int>(v.size());
        if(row != 0){
            row++;
        }
        for (int i = 0; i < length; ++i) {
            if(i != 0 && i % width == 0){
                row++;
            }
            data[row][col++] = v[i];
            if(col == width){
                col = 0;
            }
        }
    }
    return new Matrix<double>(height, width, data);
}


Matrix<int> *argmax(Matrix<double> *mat, std::string dim){
    int width = 0;
    int **data = nullptr;
    if(dim == "c"){
        width = mat->width;
        data = Create2dArray<int>(1,width);
        Matrix<double>* col = nullptr;
        for (int i = 0; i < mat->width; ++i) {
            col = mat->Col(i);
            double max = col->Get(0,0);
            int max_index = 0;
            for (int j = 0; j < col->height; ++j) {
                if(col->Get(j,0) > max){
                    max = col->Get(j,0);
                    max_index = j;
                }
            }
            data[0][i] = max_index;
            delete(col);
        }
    } else if (dim == "r"){
        width = mat->height;
        data = Create2dArray<int>(1,width);
        Matrix<double>* row = nullptr;
        for (int i = 0; i < mat->height; ++i) {
            row = mat->Row(i);
            double max = row->Get(0,0);
            int max_index = 0;
            for (int j = 0; j < row->width; ++j) {
                if(row->Get(0,j) > max){
                    max = row->Get(0,j);
                    max_index = j;
                }
            }
            data[0][i] = max_index;
            delete(row);
        }
    }
    return new Matrix<int>(1, width, data);
}

Matrix<double> *max(Matrix<double> *mat, std::string dim) {
    int width = 0;
    double **data = nullptr;
    if(dim == "c"){
        width = mat->width;
        data = Create2dArray<double>(1,width);
        Matrix<double>* col = nullptr;
        for (int i = 0; i < mat->width; ++i) {
            col = mat->Col(i);
            double max = col->Get(0,0);
            for (int j = 0; j < col->height; ++j) {
                if(col->Get(j,0) > max){
                    max = col->Get(j,0);
                }
            }
            data[0][i] = max;
            delete(col);
        }
    } else if (dim == "r"){
        width = mat->height;
        data = Create2dArray<double>(1,width);
        Matrix<double>* row = nullptr;
        for (int i = 0; i < mat->height; ++i) {
            row = mat->Row(i);
            double max = row->Get(0,0);
            for (int j = 0; j < row->width; ++j) {
                if(row->Get(0,j) > max){
                    max = row->Get(0,j);
                }
            }
            data[0][i] = max;
            delete(row);
        }
    }
    return new Matrix<double>(1, width, data);
}
//Matrix *Padding(Matrix *data){
//    Matrix *padding = new Matrix;
//    int bound = (this->Size - 1) / 2;
//    padding->width = data->width + this->Size - 1;
//    padding->height = data->height + this->Size - 1;
//
//    padding->data = new double *[padding->height];
//    for (int i = 0; i < padding->height; ++i) {
//        padding->data[i] = new double[padding->width];
//    }
//
//    for (int j = 0; j < padding->height; ++j) {
//        for (int i = 0; i < padding->width; ++i) {
//            if(j > (bound - 1) && j <= (padding->height - bound - 1) && i > (bound - 1) && i <=(padding->width - bound - 1)){
//                padding->data[j][i] = data->data[j - bound][i - bound];
//            } else {
//                padding->data[j][i] = 0;
//            }
//        }
//    }
//    return padding;
//}


