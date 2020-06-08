#include <iostream>
#include <cstdlib>
#include "matrix.hpp"
#include "load_data.hpp"
#include <windows.h>
#include "utils.h"
#include "convolution.hpp"
#include "poling.hpp"

template<class Type>
void DrawImg(Matrix<Type> *mat, int x, int y) {
    HWND wnd;    //窗口句柄
    HDC dc;    //绘图设备环境句柄
    wnd = GetForegroundWindow(); //获取窗口句柄
    dc = GetDC(wnd);    //获取绘图设备
    for (int j = 0; j < mat->height; j++) {
        for (int i = 0; i < mat->width; i++) {
            Type v = mat->data[j][i];
            SetPixel(dc, i + x, j + y, RGB(v, v, v)); //画像素点
        }
    }
    std::cout << "any key next..." << std::endl;
    getchar();
}

int main() {

    auto data = new vector<vector<Matrix<double>*>>;
    vector<unsigned int> *labels = new vector<unsigned int>;
    Load_data(R"(.\data\img_label\t10k-images.idx3-ubyte)",
         R"(.\data\img_label\t10k-labels.idx1-ubyte)",
              data,
            labels
            );


    printf("%d , %d\n", static_cast<int>(data->size()), static_cast<int>(labels->size()));

    auto x = new vector<vector<Matrix<double>*>>(data->begin(), data->begin() + 100);
    vector<Matrix<double>*> *w = new vector<Matrix<double>*>;
    w->push_back(new Matrix<double>{{1,1,1},{2,2,2},{3,3,3}});
    w->push_back(new Matrix<double>{{4,4,4},{5,5,5},{6,6,6}});
    w->push_back(new Matrix<double>{{7,7,7},{8,8,8},{9,9,9}});
    auto b = new Matrix<double>{1,2,3};
    auto conv = new Convelution(w, b);
    auto out = conv->forward(x);
    auto pool = new Pooling(2,2,2);
    pool->forward(out);







    return 0;
}