#include <iostream>
#include <cstdlib>
#include "matrix.h"
#include "load_data.hpp"
#include <windows.h>
#include "utils.h"
//#include "convolution.hpp"

void DrawImg(Matrix *mat, int x, int y) {
    HWND wnd;    //窗口句柄
    HDC dc;    //绘图设备环境句柄
    wnd = GetForegroundWindow(); //获取窗口句柄
    dc = GetDC(wnd);    //获取绘图设备
    for (int j = 0; j < mat->height; j++) {
        for (int i = 0; i < mat->width; i++) {
            double v = mat->data[j][i];
            SetPixel(dc, i + x, j + y, RGB(v, v, v)); //画像素点
        }
    }
    std::cout << "any key next..." << std::endl;
    getchar();
}

int main() {

    Matrix m = {{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
    std::cout << m.Reshape(3, -1);

//    auto data = new vector<vector<Matrix*>>;
//    vector<unsigned int> *labels = new vector<unsigned int>;
//    Load_data(R"(.\data\img_label\t10k-images.idx3-ubyte)",
//         R"(.\data\img_label\t10k-labels.idx1-ubyte)",
//              data,
//            labels
//            );
//
//    printf("%d , %d\n", static_cast<int>(data->size()), static_cast<int>(labels->size()));
//    printf("%d\n", *labels->begin());
//    DrawImg(*imgs->begin(), 100, 100);


    return 0;
}