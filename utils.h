//
// Created by Administrator on 2020/6/4.
//

#ifndef CONVELUTION_UTILS_H
#define CONVELUTION_UTILS_H

#include <vector>
#include "matrix.hpp"
//#include <windows.h>

Matrix<double> *im2col(std::vector<std::vector<Matrix<double> *>> *input_data, int filter_h, int filter_w, int stride = 1, int pad = 0);

Matrix<int> *argmax(Matrix<double> *mat, std::string dim);

Matrix<double> *max(Matrix<double> *mat, std::string dim);

Matrix<double> *sum(Matrix<double> *mat, std::string dim);

std::vector<std::string> split(std::string str, std::string pattern);

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

Matrix<double> *ReSize(Matrix<double> *src, double sigma);

double cross_entropy_error(Matrix<double> *y, Matrix<double> *t);
//template<class Type>
//void DrawImg(Matrix<Type> *mat, int x, int y) {
//    HWND wnd;    //窗口句柄
//    HDC dc;    //绘图设备环境句柄
//    wnd = GetForegroundWindow(); //获取窗口句柄
//    dc = GetDC(wnd);    //获取绘图设备
//    for (int j = 0; j < mat->height; j++) {
//        for (int i = 0; i < mat->width; i++) {
//            Type v = mat->data[j*mat->width + i];
//            SetPixel(dc, i + x, j + y, RGB(v, v, v)); //画像素点
//        }
//    }
//    std::cout << "any key next..." << std::endl;
//    getchar();
//}

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




#endif //CONVELUTION_UTILS_H
