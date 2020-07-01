//
// Created by Administrator on 2020/6/4.
//

#ifndef CONVELUTION_UTILS_H
#define CONVELUTION_UTILS_H

#include <vector>
#include "matrix.hpp"

struct Point{
    int height;
    int width;
};


Matrix<double> *im2col(std::vector<std::vector<Matrix<double> *>> *input_data, int filter_h, int filter_w, int stride = 1, int pad = 0);

Matrix<int> *argmax(Matrix<double> *mat, std::string dim);

Matrix<double> *max(Matrix<double> *mat, std::string dim);

Matrix<double> *sum(Matrix<double> *mat, std::string dim);

std::vector<std::string> split(std::string str, std::string pattern);

Matrix<double> *rand_matrix_n(int row, int col);

Matrix<double> *rand_matrix(int row, int col);

Matrix<double> *mat_exp(Matrix<double> *x);

Matrix<double> *mat_log(Matrix<double> *x);

Matrix<double> *padding(Matrix<double> *src, int pad);

void free_data(std::vector<std::vector<Matrix<double> *>> *data);

double gauss_rand();

std::string read_file(std::string file);

Matrix<double> *col2im(Matrix<double> *col, int src_w, int src_h, int filter_size, int stride);

std::string &replace_all(std::string &str, const std::string &old_value, const std::string &new_value);

void progress_bar(int per, int totle, double time, char *info);

double cubic_coeff(double x);

int _round_(double a);

Matrix<double> *ReSize(Matrix<double> *src, double sigma);

void Build_img(int sub_img_h_w, int stride, Matrix<double> *src, double min, double max, std::vector<double> &sigmas, std::vector<Matrix<double>*> &imgs, std::vector<Point> &points);

double cross_entropy_error(Matrix<double> *y, Matrix<double> *t);


template<class _Type>
_Type max(_Type *data, int len) {
    _Type max = data[0];
    for (int i = 0; i < len; ++i) {
        if (data[i] > max) {
            max = data[i];
        }
    }
    return max;
}


template<class _Type>
int max_index(_Type *data, int len) {
    _Type max = data[0];
    int index = 0;
    for (int i = 0; i < len; ++i) {
        if (data[i] > max) {
            max = data[i];
            index = i;
        }
    }
    return index;
}


//洗牌
template <class Type1, class  Type2>
void shuffle_data(std::vector<Type1> &data1, std::vector<Type2> &data2){
    int n = static_cast<int>(data1.size());
    int T = 1000;
    int i,j;
    srand(static_cast<unsigned int>(time(nullptr)));
    while(T--){
        i = rand()%n;
        j = rand()%n;
        std::swap(data1[i], data1[j]);
        std::swap(data2[i], data2[j]);
    }
}



#endif //CONVELUTION_UTILS_H
