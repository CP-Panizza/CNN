//
// Created by Administrator on 2020/6/4.
//

#ifndef CONVELUTION_MATRIX_HPP
#define CONVELUTION_MATRIX_HPP


#include <fstream>
#include <cstdlib>
#include <memory.h>
#include <vector>
#include <iostream>
#include <cmath>
//
///**
// * M:第一个矩阵height
// * N:第二个矩阵width
// * K:第一个矩阵的width第二个矩阵的height
// * strideA: 第一个矩阵width
// * strideB: 第二个矩阵width
// * strideC: 第三个矩阵width
// */
//
//
//template<class Dtype>
//void mm_generate(Dtype *matA, Dtype *matB, Dtype *matC,
//                 int M, int N, int K,
//                 int strideA, int strideB, int strideC) {
//     Dtype sum = 0;
//    for (int i = 0; i < M; i++) {
//        for (int j = 0; j < N; j++) {
//            sum = 0;
//            for (int k = 0; k < K; k++) {
//                if(matA[i * strideA + k] != 0){
//                    sum += matA[i * strideA + k] * matB[k * strideB + j];
//                }
//            }
//            matC[i * strideC + j] = sum;
//        }
//    }
//}
//
//
//
//template<class Dtype>
//void mm_winograd(Dtype *matA, Dtype *matB, Dtype *matC, int M, int N, int K, int strideA,
//                 int strideB, int strideC) {
//    if ((M <= 64) || (M % 2 != 0 || N % 2 != 0 || K % 2 != 0)) {
//        return mm_generate<Dtype>(matA, matB, matC, M, N, K, strideA, strideB, strideC);
//    }
//    memset(matC, 0, M * strideC * sizeof(Dtype));
//    int offset = 0;
//
//    std::vector<Dtype> S1((M / 2) * (K / 2));
//    std::vector<Dtype> S2((M / 2) * (K / 2));
//    std::vector<Dtype> S3((M / 2) * (K / 2));
//    std::vector<Dtype> S4((M / 2) * (K / 2));
//    for (int i = 0; i < M / 2; i++) {
//        for (int j = 0; j < K / 2; j++) {
//            const int idx = i * K / 2 + j;
//            //S1 = A21 + A22
//            S1[idx] = matA[(i + M / 2) * strideA + j] + matA[(i + M / 2) * strideA + j + K / 2];
//            //S2 = S1 - A11
//            S2[idx] = S1[idx] - matA[i * strideA + j];
//            //S3 = A11 - A21
//            S3[idx] = matA[i * strideA + j] - matA[(i + M / 2) * strideA + j];
//            //S4 = A12 - S2
//            S4[idx] = matA[i * strideA + j + K / 2] - S2[idx];
//        }
//    }
//    std::vector<Dtype> T1((K / 2) * (N / 2));
//    std::vector<Dtype> T2((K / 2) * (N / 2));
//    std::vector<Dtype> T3((K / 2) * (N / 2));
//    std::vector<Dtype> T4((K / 2) * (N / 2));
//    for (int i = 0; i < K / 2; i++) {
//        for (int j = 0; j < N / 2; j++) {
//            const int idx = i * N / 2 + j;
//            //T1 = B21 - B11
//            T1[idx] = matB[(i + K / 2) * strideB + j] - matB[i * strideB + j];
//            //T2 = B22 - T1
//            T2[idx] = matB[(i + K / 2) * strideB + j + N / 2] - T1[idx];
//            //T3 = B22 - B12
//            T3[idx] = matB[(i + K / 2) * strideB + j + N / 2] - matB[i * strideB + j + N / 2];
//            //T4 = T2 - B21
//            T4[idx] = T2[idx] - matB[(i + K / 2) * strideB + j];
//        }
//    }
//
//    //M1 = A11*B11
//    std::vector<Dtype> M1((M / 2) * (N / 2));
//    {
//        memset(&M1[0], 0, M1.size() * sizeof(Dtype));
//        mm_winograd<Dtype>(matA, matB, &M1[0], M / 2, N / 2, K / 2,
//                           strideA, strideB, N / 2);
//    }
//
//    //M2 = A12*B21
//    std::vector<Dtype> M2((M / 2) * (N / 2));
//    {
//        memset(&M2[0], 0, M2.size() * sizeof(Dtype));
//        mm_winograd<Dtype>(matA + K / 2, matB + K * strideB / 2, &M2[0], M / 2, N / 2, K / 2,
//                           strideA, strideB, N / 2);
//    }
//
//    //M3 = S4*B22
//    std::vector<Dtype> M3((M / 2) * (N / 2));
//    {
//        memset(&M3[0], 0, M3.size() * sizeof(Dtype));
//        mm_winograd<Dtype>(&S4[0], matB + K * strideB / 2 + N / 2, &M3[0], M / 2, N / 2, K / 2,
//                           K / 2, strideB, N / 2);
//    }
//
//    //M4 = A22*T4
//    std::vector<Dtype> M4((M / 2) * (N / 2));
//    {
//        memset(&M4[0], 0, M4.size() * sizeof(Dtype));
//        mm_winograd<Dtype>(matA + M * strideA / 2 + K / 2, &T4[0], &M4[0], M / 2, N / 2, K / 2,
//                           strideA, N / 2, N / 2);
//    }
//
//    //M5 = S1*T1
//    std::vector<Dtype> M5((M / 2) * (N / 2));
//    {
//        memset(&M5[0], 0, M5.size() * sizeof(Dtype));
//        mm_winograd<Dtype>(&S1[0], &T1[0], &M5[0], M / 2, N / 2, K / 2,
//                           K / 2, N / 2, N / 2);
//    }
//
//    //M6 = S2*T2
//    std::vector<Dtype> M6((M / 2) * (N / 2));
//    {
//        memset(&M6[0], 0, M6.size() * sizeof(Dtype));
//        mm_winograd<Dtype>(&S2[0], &T2[0], &M6[0], M / 2, N / 2, K / 2,
//                           K / 2, N / 2, N / 2);
//    }
//
//    //M7 = S3*T3
//    std::vector<Dtype> M7((M / 2) * (N / 2));
//    {
//        memset(&M7[0], 0, M7.size() * sizeof(Dtype));
//        mm_winograd<Dtype>(&S3[0], &T3[0], &M7[0], M / 2, N / 2, K / 2,
//                           K / 2, N / 2, N / 2);
//    }
//
//    for (int i = 0; i < M / 2; i++) {
//        for (int j = 0; j < N / 2; j++) {
//            const int idx = i * N / 2 + j;
//            //U1 = M1 + M2
//            const auto U1 = M1[idx] + M2[idx];
//            //U2 = M1 + M6
//            const auto U2 = M1[idx] + M6[idx];
//            //U3 = U2 + M7
//            const auto U3 = U2 + M7[idx];
//            //U4 = U2 + M5
//            const auto U4 = U2 + M5[idx];
//            //U5 = U4 + M3
//            const auto U5 = U4 + M3[idx];
//            //U6 = U3 - M4
//            const auto U6 = U3 - M4[idx];
//            //U7 = U3 + M5
//            const auto U7 = U3 + M5[idx];
//
//            //C11 = U1
//            matC[i * strideC + j] = U1;
//            //C12 = U5
//            matC[i * strideC + j + N / 2] = U5;
//            //C21 = U6
//            matC[(i + M / 2) * strideC + j] = U6;
//            //C22 = U7
//            matC[(i + M / 2) * strideC + j + N / 2] = U7;
//        }
//    }
//}

//template<class Dtype>
//void mm_strassen(Dtype* matA, Dtype* matB, Dtype* matC,  int M,  int N,  int K,  int strideA,  int strideB,  int strideC)
//{
//    if ((M <= 64) || (M%2 != 0 ||N%2 != 0 ||K%2!=0))
//    {
//        return mm_generate<Dtype>(matA, matB, matC, M, N, K, strideA, strideB, strideC);
//    }
//    memset(matC, 0, M*strideC*sizeof(Dtype));
//    int offset = 0;
//
//    //M1 = (A11+A22)*(B11+B22)
//    std::vector<Dtype> M1((M / 2) * (N / 2));
//    {
//        memset(&M1[0], 0, M1.size()*sizeof(Dtype));
//        //M1_0 = (A11+A22)
//        std::vector<Dtype> M1_0((M / 2) * (K / 2));
//        offset = M*strideA / 2 + K / 2;
//        for (int i = 0; i < M / 2; i++)
//        {
//            for (int j = 0; j < K/2; j++)
//            {
//                const int baseIdx = i*strideA + j;
//                M1_0[i*K/2+j] = matA[baseIdx] + matA[baseIdx + offset];
//            }
//        }
//        //M1_1 = (B11+B22)
//        std::vector<Dtype> M1_1((K / 2) * (N / 2));
//        offset = K*strideB / 2 + N / 2;
//        for (int i = 0; i < K / 2; i++)
//        {
//            for (int j = 0; j < N / 2; j++)
//            {
//                const int baseIdx = i*strideB + j;
//                M1_1[i*N/2+j] = matB[baseIdx] + matB[baseIdx + offset];
//            }
//        }
//        mm_strassen<Dtype>(&M1_0[0], &M1_1[0], &M1[0], M / 2, N / 2, K / 2,
//                    K/2,N/2,N/2);
//    }
//
//    //M2 = (A21+A22)*B11
//    std::vector<Dtype> M2((M / 2) * (N / 2));
//    {
//        memset(&M2[0], 0, M2.size()*sizeof(Dtype));
//        //M2_0 = (A21+A22)
//        std::vector<Dtype> M2_0((M / 2) * (K / 2));
//        offset = K / 2;
//        for (int i = M / 2; i < M; i++)
//        {
//            for (int j = 0; j < K / 2; j++)
//            {
//                const int baseIdx = i*strideA + j;
//                M2_0[(i-M/2)*K/2+j] = matA[baseIdx] + matA[baseIdx + offset];
//            }
//        }
//        //M2_2 = B11
//        mm_strassen<Dtype>(&M2_0[0], &matB[N / 2], &M2[0], M / 2, N / 2, K / 2,
//                    K / 2, strideB, N / 2);
//    }
//
//    //M3 = A11*(B12-B22)
//    std::vector<Dtype> M3((M / 2) * (N / 2));
//    {
//        memset(&M3[0], 0, M3.size()*sizeof(Dtype));
//        //M3_0 = A11
//        //M3_1 = (B12-B22)
//        std::vector<Dtype> M3_1((K / 2) * (N / 2));
//        offset = K*strideB / 2;
//        for (int i = 0; i < K/2; i++)
//        {
//            for (int j = N/2; j < N; j++)
//            {
//                const int baseIdx = i*strideB + j;
//                M3_1[i*N/2+j-N/2] = matB[baseIdx] - matB[baseIdx + offset];
//            }
//        }
//        mm_strassen<Dtype>(matA, &M3_1[0], &M3[0], M / 2, N / 2, K / 2,
//                    strideA, N / 2, N / 2);
//    }
//
//    //M4 = A22*(B21-B11)
//    std::vector<Dtype> M4((M / 2) * (N / 2));
//    {
//        memset(&M4[0], 0, M4.size()*sizeof(Dtype));
//        //M4_0 = A22
//        //M4_1 = (B12-B22)
//        std::vector<float> M4_1((K / 2) * (N / 2));
//        offset = K*strideB / 2;
//        for (int i = 0; i < K / 2; i++)
//        {
//            for (int j = N / 2; j < N; j++)
//            {
//                const int baseIdx = i*strideB + j;
//                M4_1[i*N/2+j-N/2] = matB[baseIdx + offset] - matB[baseIdx];
//            }
//        }
//        mm_strassen<Dtype>(matA + M*strideA / 2 + K / 2, &M4_1[0], &M4[0], M / 2, K / 2, N / 2,
//                    strideA, N / 2, N / 2);
//    }
//
//    //M5 = (A11+A12)*B22
//    std::vector<Dtype> M5((M / 2) * (N / 2));
//    {
//        memset(&M5[0], 0, M5.size()*sizeof(Dtype));
//        //M5_0 = (A11+A12)
//        std::vector<Dtype> M5_0((M / 2) * (K / 2));
//        offset = K / 2;
//        for (int i = 0; i < M/2; i++)
//        {
//            for (int j = 0; j < K / 2; j++)
//            {
//                const int baseIdx = i*strideA + j;
//                M5_0[i*K / 2 + j] = matA[baseIdx] + matA[baseIdx + offset];
//            }
//        }
//        //M5_1 = B22
//        mm_strassen<Dtype>(&M5_0[0], &matB[K*strideB / 2 + N / 2], &M5[0], M / 2, N / 2, K / 2,
//                    K / 2, strideB, N / 2);
//    }
//
//    //M6 = (A21-A11)*(B11+B12)
//    std::vector<Dtype> M6((M / 2) * (N / 2));
//    {
//        memset(&M6[0], 0, M6.size()*sizeof(Dtype));
//        //M6_0 = (A21-A11)
//        std::vector<Dtype> M6_0((M / 2) * (K / 2));
//        offset = K*N / 2;
//        for (int i = 0; i < M / 2; i++)
//        {
//            for (int j = 0; j < K/2; j++)
//            {
//                const int baseIdx = i*strideA + j;
//                M6_0[i*K/2+j] = matA[baseIdx + offset] - matA[baseIdx];
//            }
//        }
//        //M6_1 = (B11+B12)
//        std::vector<Dtype> M6_1((K / 2) * (N / 2));
//        offset = N / 2;
//        for (int i = 0; i < K / 2; i++)
//        {
//            for (int j = 0; j < N/2; j++)
//            {
//                const int baseIdx = i*strideB + j;
//                M6_1[i*N/2+j] = matB[baseIdx] + matB[baseIdx + offset];
//            }
//        }
//        mm_strassen<Dtype>(&M6_0[0], &M6_1[0], &M6[0], M / 2, N / 2, K / 2,
//                    K / 2, N / 2, N / 2);
//    }
//
//    //M7 = (A12-A22)*(B21+B22)
//    std::vector<Dtype> M7((M / 2) * (N / 2));
//    {
//        memset(&M7[0], 0, M7.size()*sizeof(Dtype));
//        //M7_0 = (A12-A22)
//        std::vector<float> M7_0((M / 2) * (K / 2));
//        offset = M*strideA / 2;
//        for (int i = 0; i < M / 2; i++)
//        {
//            for (int j = K/2; j < K; j++)
//            {
//                const int baseIdx = i*strideA + j;
//                M7_0[i*K / 2 + j - K / 2] = matA[baseIdx] - matA[baseIdx + offset];
//            }
//        }
//        //M7_1 = (B21+B22)
//        std::vector<Dtype> M7_1((K / 2) * (N / 2));
//        offset = N / 2;
//        for (int i = K/2; i < K; i++)
//        {
//            for (int j = 0; j < N / 2; j++)
//            {
//                const int baseIdx = i*strideB + j;
//                M7_1[(i-K/2)*N / 2 + j] = matB[baseIdx] + matB[baseIdx + offset];
//            }
//        }
//        mm_strassen<Dtype>(&M7_0[0], &M7_1[0], &M7[0], M / 2, N / 2, K / 2,
//                    K / 2, N / 2, N / 2);
//    }
//    for (int i = 0; i < M / 2;i++)
//    {
//        for (int j = 0; j < N / 2;j++)
//        {
//            const int idx = i*N / 2 + j;
//            //C11 = M1+M4-M5+M7
//            matC[i*strideC + j] = M1[idx] + M4[idx] - M5[idx] + M7[idx];
//            //C12 = M3+M5
//            matC[i*strideC + j + N/2] = M3[idx] + M5[idx];
//            //C21 = M2+M4
//            matC[(i+M/2)*strideC + j] = M2[idx] + M4[idx];
//            //C22 = M1-M2+M3+M6
//            matC[(i+M/2)*strideC + j + N/2] = M1[idx] - M2[idx] + M3[idx] + M6[idx];
//        }
//    }
//}




template<class Type>
Type *CreateArray(int row, int col) {
    Type *array = new Type[row*col];
    memset(array,0, sizeof(Type) * col * row);
    return array;
}

template<class Type>
class Matrix {
public:
    int width;
    int height;
    Type *data = nullptr;


    Matrix() {}

    ~Matrix() {
        delete[](this->data);
    }

    Matrix(int h, int w) {
        this->data = CreateArray<Type>(h, w);
        this->width = w;
        this->height = h;
    }


    Matrix(const Matrix &mat) {
        std::cout << "copy run" << std::endl;
        this->width = mat.width;
        this->height = mat.height;
        this->data = CreateArray<Type>(this->height, this->width);
        for (int j = 0; j < this->height; ++j) {
            for (int i = 0; i < this->width; ++i) {
                this->Set(j , i, mat.Get(j, i));
            }
        }
    }

    Matrix(const std::initializer_list<std::initializer_list<Type >> &inList) {
        this->height = static_cast<int>(inList.size());
        this->width = static_cast<int>(inList.begin()->size());
        this->data = CreateArray<Type>(this->height, this->width);
        int i = 0, j = 0;
        for (auto &x :inList) {
            for (auto &y: x) {
                this->Set(i,j,y);
                j++;
            }
            j = 0;
            i++;
        }
    }

    Matrix(const std::initializer_list<Type> &inList) {
        this->width = static_cast<int>(inList.size());
        this->height = 1;
        this->data = CreateArray<Type>(this->height, this->width);
        int i = 0;
        for (auto &x :inList) {
            this->data[i] = x;
            i++;
        }
    }


    Matrix(int h, int w, Type *_data) {
        this->width = w;
        this->height = h;
        this->data = _data;
    }

    void Out() {
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                std::cout << this->Get(i, j) << ", ";
            }
            std::cout << "\n";
        }
    }

    Matrix<Type> *Copy() {
        auto t = new Matrix(this->height, this->width);
        for (int j = 0; j < this->height; ++j) {
            for (int i = 0; i < this->width; ++i) {
                t->Set(j ,i, this->Get(j,i));
            }
        }
        return t;
    }

    void WriteImg(const std::string name) {
        std::ofstream f(name);
        if (f.is_open()) {
            f << "P2\n" << this->width << " " << this->height << "\n255\n";
            int k = 1;
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    if (k % 10) {
                        f << int(this->Get(i,j)) << " ";
                    } else {
                        f << int(this->Get(i,j)) << "\n";
                    }
                    k++;
                }
            }
        } else {
            perror("open outfile err!");
        }
        f.close();
    }

    void Set(int row, int col, Type val) {
        if (col > this->width || row > this->height) {
            perror("function Set beyond matrix bound!");
            exit(-1);
        }
        this->data[row*this->width + col] = val;
    }

    Type Get(int row, int col) const {
        if (col > this->width || row > this->height) {
            perror("function Get beyond matrix bound!");
            exit(-1);
        }
        return this->data[row*this->width + col];
    }

    Matrix<Type> *T() {
        auto t_mat = new Matrix<Type>(this->width, this->height);
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                t_mat->Set(j,i, this->Get(i,j));
            }
        }
        return t_mat;
    }

    Matrix<Type> *Row(int index) {
        if (index >= this->height || index < 0) {
            printf("err: index %d beyond\n", index);
            exit(-1);
        }
        auto data = new Type[this->width];
        for (int i = 0; i < this->width; ++i) {
            data[i] = this->Get(index, i);
        }
        Matrix<Type> *m = new Matrix<Type>(1, this->width, data);
        return m;
    }


    Matrix<Type> *Col(int index) {
        if (index >= this->width || index < 0) {
            printf("err: index %d beyond\n", index);
            exit(-1);
        }
        auto data = new Type[this->height];
        for (int i = 0; i < this->height; ++i) {
            data[i] = this->Get(i, index);
        }
        Matrix<Type> *m = new Matrix<Type>(this->height, 1, data);
        return m;
    }

    Type *operator[](int index) {
        return this->data[index];
    }

    Matrix<Type> *operator+(Type a) {
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i*this->width + j] = m->data[i*this->width + j] + a;
            }
        }
        return m;
    }

    Matrix<Type> *operator*(Type a) {
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i*this->width + j] = m->data[i*this->width + j] * a;
            }
        }
        return m;
    }


    Matrix<Type> *operator+(Matrix<Type> *a) {
        if (a->height == this->height && a->width == this->width) {
            auto data = CreateArray<Type>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i*this->width + j] = this->data[i*this->width + j] + a->data[i*this->width + j];
                }
            }
            return new Matrix<Type>(this->height, this->width, data);
        }

        if (a->height != 1 || a->width != this->width) {
            printf("file: %s function: %s line: %d dim not match: %d x %d --- %d x %d", __FILE__, __FUNCTION__,
                   __LINE__, this->height, this->width, a->height, a->width);
            exit(-1);
        }
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                m->data[i*this->width + j] = m->data[i*this->width + j] + a->data[j];
            }
        }
        return m;
    }


    Matrix<Type> *operator-(Matrix<Type> *a) {

        if (a->height == this->height && a->width == this->width) {
            auto data = CreateArray<Type>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i*this->width + j] = this->data[i*this->width + j] - a->data[i*this->width + j];
                }
            }
            return new Matrix<Type>(this->height, this->width, data);
        }


        if (a->height != 1 || a->width != this->width) {
            printf("file: %s function: %s line: %d dim not match: %d x %d --- %d x %d", __FILE__, __FUNCTION__,
                   __LINE__, this->height, this->width, a->height, a->width);
            exit(-1);
        }
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                m->data[i*this->width + j] = m->data[i*this->width + j] - a->data[j];
            }
        }
        return m;
    }

    Matrix<Type> *operator/(Matrix<Type> *a) {
        if (a->height == 1 && a->width == this->width) {
            auto data = CreateArray<double>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i*this->width + j] = this->data[i*this->width + j] / a->data[j];
                }
            }
            return new Matrix<double>(this->height, this->width, data);
        } else if (a->height == this->height && a->width == this->width) {
            auto out = this->Copy();
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    Type val = a->data[i*this->width + j];
                    if (val == 0) {
                        printf("file: %s function: %s line: %d div zero.", __FILE__, __FUNCTION__,
                               __LINE__);
                        exit(-1);
                    }
                    out->data[i*this->width + j] = this->data[i*this->width + j] / val;
                }
            }
            return out;
        }
        printf("file: %s function: %s line: %d dim not match: %d x %d --- %d x %d", __FILE__, __FUNCTION__,
               __LINE__, this->height, this->width, a->height, a->width);
        exit(-1);
    }

    Matrix<Type> *operator/(Type a) {
        if (a == 0) {
            printf("file: %s function: %s line: %d div zero.", __FILE__, __FUNCTION__,
                   __LINE__);
            exit(-1);
        }
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->Set(i,j, m->Get(i,j) / a);
            }
        }
        return m;
    }

    /**
     * 矩阵求幂次方
     * @param e
     * @return
     */
    Matrix<Type> *mat_pow(double e) {
        auto out = this->Copy();
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                out->Set(i, j, pow(this->Get(i, j), e));
            }
        }
        return out;
    }

//    /**
//     * winograd算法
//     * @param m
//     * @return
//     */
//    Matrix<Type> *Dot_(Matrix<Type> *m) {
//        if (this->width != m->height) {
//            printf("file: %s function: %s line: %d dim not match: %d x %d --- %d x %d", __FILE__, __FUNCTION__,
//                   __LINE__, this->height, this->width, m->height, m->width);
//            exit(-1);
//        }
//        Type *matC = CreateArray<Type>(this->height, m->width);
//        mm_winograd<Type>(this->data, m->data, matC, this->height, m->width, this->width, this->width, m->width, m->width);
//        return new Matrix<Type>(this->height, m->width, matC);
//    }

    //普通优化
    Matrix<Type> *Dot(Matrix<Type> *m) {
        if (this->width != m->height) {
            printf("file: %s function: %s line: %d dim not match: %d x %d --- %d x %d", __FILE__, __FUNCTION__,
                   __LINE__, this->height, this->width, m->height, m->width);
            exit(-1);
        }
        auto out = new Matrix<double>(this->height, m->width);
        Type sum = 0;
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < m->width; j++) {
                sum = 0;
                for (int k = 0; k < m->height; k++) {
                    if(this->data[i * this->width + k] != 0){
                        sum += this->data[i * this->width + k] * m->data[k * m->width + j];
                    }
                }
                out->data[i * m->width + j] = sum;
            }
        }
        return out;
    }


    Matrix<Type> *Reshape(int row, int col) {
        if (row == -1) {
            row = (this->width * this->height) / col;
        } else if (col == -1) {
            col = (this->width * this->height) / row;
        }
        if (row * col != this->width * this->height) {
            printf("file: %s function: %s line: %d element num is not match!\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
        Matrix<Type> *line = nullptr;
        auto all = new std::vector<Type>;
        for (int i = 0; i < this->height; ++i) {
            line = this->Row(i);
            for (int j = 0; j < line->width; ++j) {
                all->push_back(line->Get(0, j));
            }
            delete (line);
        }
        int index = 0;
        auto data = CreateArray<Type>(row, col);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[i * col + j] = (*all)[index++];
            }
        }
        delete (all);
        return new Matrix<Type>(row, col, data);
    }


    /**
     * 截取子矩阵
     * @param row
     * @param col
     * @param width
     * @param height
     * @return
     */
    Matrix<Type> *SubMat(int row, int col, int width, int height) {
        if (row + height > this->height || col + width > this->width) {
            std::cout << "can not sub mat:(" << this->height << "," << this->width << ")";
            exit(-1);
        }
        auto mat = new Matrix<Type>(height, width);
        int c = 0, r = 0;
        for (int i = row; i < row + height; ++i) {
            c = 0;
            for (int j = col; j < col + width; ++j) {
                mat->Set(r, c++, this->Get(i, j));
            }
            r++;
        }
        return mat;
    }

    //转成一维数组
    Type *to_line() {
        Type *line = new Type[this->height * this->width];
        memcpy(line, this->data, sizeof(Type) * this->height * this->width);
        return line;
    }

    /*
     * 外填充pad圈0
     */
    Matrix<Type> *padding(int pad) {
        int width = this->width + (2 * pad);
        int height = this->height + (2 * pad);
        auto *pad_mat = new Matrix<Type>(height, width);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (i >= pad && i < height - pad && j >= pad && j < width - pad) {
                    pad_mat->Set(i, j, this->Get(i - pad, j - pad));
                }
            }
        }
        return pad_mat;
    }

    /*
     * 矩阵旋转180度
     */
    Matrix<Type> *rot180() {
        auto rot = this->Copy();
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                rot->Set(rot->height - 1 - i, rot->width - 1 - j, this->Get(i, j));
            }
        }
        return rot;
    }


    //矩阵求和
    Type mat_sum() {
        Type sum = 0;
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                sum += this->Get(i, j);
            }
        }
        return sum;
    }


    /**
     * 填充0
     * @param stride：[h_stride, w_stride],行与列上分别填充的步长
     * @return
     */
    Matrix<Type> *inner_padding(std::vector<int> &stride) {
        int h_stride = stride[0];
        int w_stride = stride[1];
        if (h_stride < 0 || w_stride < 0) {
            perror("stride cannot less than zero.\n");
            exit(-1);
        }

        int out_width = this->width * (w_stride + 1) - w_stride;
        int out_height = this->height * (h_stride + 1) - h_stride;

        auto out = new Matrix<Type>(out_height, out_width);
        int m, n;
        for (int i = 0, m = 0; i < out_height; i += h_stride + 1, m++) {
            for (int j = 0, n = 0; j < out_width; j += w_stride + 1, n++) {
                out->Set(i, j, this->Get(m, n));
            }
        }

        return out;
    }

    /**
     * 上下反转矩阵
     * @return
     */
    Matrix<Type> *UD_reversal() {
        auto ud = this->Copy();
        for (int i = 0; i < ud->width; ++i) {
            for (int j = 0; j < ud->height / 2; ++j) {
                Type down = this->Get(this->height - j - 1, i);
                Type up = ud->Get(j, i);
                ud->Set(j, i, down);
                ud->Set(this->height - j - 1, i, up);
            }
        }
        return ud;
    }

    /**
     * 矩阵左右反转
     * @return
     */
    Matrix<Type> *LR_reversal() {
        auto lr = this->Copy();
        for (int i = 0; i < lr->height; ++i) {
            for (int j = 0; j < lr->width / 2; ++j) {
                Type right = this->Get(i, this->width - j - 1);
                Type left = lr->Get(i, j);
                lr->Set(i, j, right);
                lr->Set(i, this->width - j - 1, left);
            }
        }
        return lr;
    }

};


template<class Type>
std::ostream &operator<<(std::ostream &os, Matrix<Type> *matrix) {
    for (int i = 0; i < matrix->height; ++i) {
        for (int j = 0; j < matrix->width; ++j) {
            os << matrix->Get(i, j) << ", ";
        }
        os << "\n";
    }
    return os;
}

#endif //CONVELUTION_MATRIX_HPP
