//
// Created by Administrator on 2020/6/3.
//

#include <c++/ctime>
#include "utils.h"
#include <cmath>

Matrix<double> *im2col(std::vector<std::vector<Matrix<double> *>> *input_data, int filter_h, int filter_w, int stride, int pad) {
    int N = static_cast<int>(input_data->size()), C = static_cast<int>(((*input_data)[0]).size()), H = ((*input_data)[0])[0]->height, W = ((*input_data)[0])[0]->width;
    int out_h = (H + 2 * pad - filter_h) / stride + 1;
    int out_w = (W + 2 * pad - filter_w) / stride + 1;

    auto input_data_copy = new std::vector<std::vector<Matrix<double> *>> ;
    for (int i = 0; i < N; ++i) {
        input_data_copy->push_back(std::vector<Matrix<double> *>(C));
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < C; ++j) {
            (*input_data_copy)[i][j] = padding((*input_data)[i][j], pad);
        }
    }

    int padded_img_h = ((*input_data_copy)[0])[0]->height;
    int padded_img_w = ((*input_data_copy)[0])[0]->width;
    std::vector<std::vector<double>> result;
    int i, j, n, k, m;
    for (auto &vec: *input_data_copy) {
            std::vector<double> line;
            for ( i = 0; i <= padded_img_h - filter_h; i += stride) {
                for ( j = 0; j <= padded_img_w - filter_w; j += stride) {
                    for ( n = 0; n < vec.size(); n++) {
                        for ( k = 0; k < filter_h; ++k) {
                            for ( m = 0; m < filter_w; ++m) {
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
        for (i = 0; i < length; ++i) {
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

Matrix<double> *sum(Matrix<double> *mat, std::string dim) {
    int width = 0;
    double **data = nullptr;
    if(dim == "c"){
        width = mat->width;
        data = Create2dArray<double>(1,width);
        Matrix<double>* col = nullptr;
        for (int i = 0; i < mat->width; ++i) {
            col = mat->Col(i);
            double sum = 0;
            for (int j = 0; j < col->height; ++j) {
                sum += col->Get(j, 0);
            }
            data[0][i] = sum;
            delete(col);
        }
    } else if (dim == "r"){
        width = mat->height;
        data = Create2dArray<double>(1,width);
        Matrix<double>* row = nullptr;
        for (int i = 0; i < mat->height; ++i) {
            row = mat->Row(i);
            double sum = 0;
            for (int j = 0; j < row->width; ++j) {
                sum += row->Get(0, j);
            }
            data[0][i] = sum;
            delete(row);
        }
    }
    return new Matrix<double>(1, width, data);
}


std::vector<std::string> split(std::string str, std::string pattern) {
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    auto size = static_cast<int>(str.size());
    for (int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = static_cast<int>(pos + pattern.size() - 1);
        }
    }
    return result;
}


Matrix<double> *rand_matrix(int row, int col){
    auto data = Create2dArray<double>(row, col);
    double min = 0;
    double sum = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = gauss_rand();
            if(data[i][j] < min){
                min = data[i][j];
            }
        }
    }
    auto temp = new Matrix<double>(row, col, data);
    return temp;
//    if(min < 0){
//        auto temp1 = temp->operator+(-min);
//        sum = temp1->mat_sum();
//        auto out = temp1->operator/(sum);
//        delete(temp);
//        delete(temp1);
//        return out;
//    }
//    sum = temp->mat_sum();
//    return temp->operator/(sum);
}


Matrix<double> *mat_exp(Matrix<double> *x){
    auto data = Create2dArray<double>(x->height, x->width);
    double v;
    for (int i = 0; i < x->height; ++i) {
        for (int j = 0; j < x->width; ++j) {
            v = x->Get(i, j);
            data[i][j] = exp(v);
        }
    }
    return new Matrix<double>(x->height, x->width, data);
}




Matrix<double> *padding(Matrix<double> *src, int pad){
    int width = src->width + (2 * pad);
    int height = src->height + (2 * pad);
    auto *pad_mat = new Matrix<double>(height, width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(i>=pad && i<height-pad && j>=pad && j<width-pad){
                pad_mat->Set(i, j, src->Get(i-pad, j-pad));
            }
        }
    }
    return pad_mat;
}



void free_data(std::vector<std::vector<Matrix<double> *>> *data) {
    for (int i = 0; i < data->size(); ++i) {
        for (int j = 0; j < (*data)[i].size(); ++j) {
            delete ((*data)[i][j]);
        }
    }
    delete (data);
}


//产生高斯分布随机数
double gauss_rand(){
    static double V1, V2, S;
    static int phase = 0;
    double X;
    if(phase == 0){
        do{
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1*V1 + V2*V2;
        }while(S>=1 || S==0);
        X = V1 *sqrt(-2 * log(S) / S);
    } else {
        X = V2 *sqrt(-2 * log(S) / S);
    }
    phase = 1 - phase;

    return X;
}