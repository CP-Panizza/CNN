#include <iostream>
#include <cstdlib>
#include "matrix.hpp"
#include "load_data.hpp"
#include <windows.h>

void DrawImg(Matrix *mat, int x, int y){
    HWND wnd;	//窗口句柄
    HDC dc;	//绘图设备环境句柄
    wnd = GetForegroundWindow(); //获取窗口句柄
    dc = GetDC(wnd);	//获取绘图设备
    for(int j=0;j< mat->height;j++) {
        for (int i = 0; i < mat->width; i++) {
            float v = mat->data[j][i];
            SetPixel(dc, i + x, j + y, RGB(v,v,v)); //画像素点
        }
    }
    std::cout << "any key next..." << std::endl;
    getchar();
}

int main() {


//    vector<Matrix *> *imgs = new vector<Matrix *>;
//    vector<unsigned int> *labels = new vector<unsigned int>;
//    load("C:\\Users\\Administrator\\Desktop\\Work_bench\\cpp\\Convelution\\cmake-build-debug\\data\\img_label\\t10k-images.idx3-ubyte",
//            "C:\\Users\\Administrator\\Desktop\\Work_bench\\cpp\\Convelution\\cmake-build-debug\\data\\img_label\\t10k-labels.idx1-ubyte",
//            imgs,
//            labels
//            );
//
//    printf("%d , %d", imgs->size(), labels->size());
//    printf("%d", *labels->begin());
//    DrawImg(*imgs->begin(), 10, 10);
    return 0;
}