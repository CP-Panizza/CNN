//
// Created by Administrator on 2020/6/4.
//

#ifndef CONVELUTION_MATRIX_HPP
#define CONVELUTION_MATRIX_HPP

#include <fstream>
#include <cstdlib>
#include <memory.h>
#include <c++/vector>
#include <c++/iostream>
#include <cmath>

template<class Type>
Type **Create2dArray(int row, int col) {
    Type **array = new Type *[row];
    for (int i = 0; i < row; ++i) {
        array[i] = new Type[col];
        memset(array[i], 0, sizeof(Type) * col);
    }
    return array;
}

template<class Type>
class Matrix {
public:
    int width;
    int height;
    Type **data = nullptr;


    Matrix() {}

    ~Matrix() {
        for (int i = 0; i < this->height; ++i) {
            delete[](this->data[i]);
        }
        delete[](this->data);
    }

    Matrix(int h, int w) {
        this->data = Create2dArray<Type>(h, w);
        this->width = w;
        this->height = h;
    }


    Matrix(const Matrix &mat) {
        std::cout << "copy run" << std::endl;
        this->width = mat.width;
        this->height = mat.height;
        this->data = Create2dArray<Type>(this->height, this->width);
        for (int j = 0; j < this->height; ++j) {
            for (int i = 0; i < this->width; ++i) {
                this->data[j][i] = mat.Get(j, i);
            }
        }
    }

    Matrix(const std::initializer_list<std::initializer_list<Type >> &inList) {
        this->height = static_cast<int>(inList.size());
        this->width = static_cast<int>(inList.begin()->size());
        this->data = Create2dArray<Type>(this->height, this->width);
        int i = 0, j = 0;
        for (auto &x :inList) {
            for (auto &y: x) {
                this->data[i][j] = y;
                j++;
            }
            j = 0;
            i++;
        }
    }

    Matrix(const std::initializer_list<Type> &inList) {
        this->width = static_cast<int>(inList.size());
        this->height = 1;
        this->data = Create2dArray<Type>(this->height, this->width);
        int i = 0;
        for (auto &x :inList) {
            this->data[0][i] = x;
            i++;
        }
    }

    Matrix(int h, int w, Type *_data) {
        this->data = Create2dArray<Type>(h, w);
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                this->data[j][i] = _data[w * j + i];
            }
        }

        this->width = w;
        this->height = h;
    }

    Matrix(int h, int w, Type **_data) {
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
        auto t = new Matrix;
        t->width = this->width;
        t->height = this->height;
        t->data = Create2dArray<Type>(this->height, this->width);
        for (int j = 0; j < this->height; ++j) {
            for (int i = 0; i < this->width; ++i) {
                t->Set(j, i, this->Get(j, i));
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
                        f << this->data[i][j] << " ";
                    } else {
                        f << this->data[i][j] << "\n";
                    }
                    k++;
                }
            }
        } else {
            perror("open outfile err!");
        }
        f.close();
    }

    bool Set(int row, int col, Type val) {
        if (col > this->width || row > this->height) return false;
        this->data[row][col] = val;
        return true;
    }

    Type Get(int row, int col) const {
        if (col > this->width || row > this->height) return -1;
        return this->data[row][col];
    }

    Matrix<Type> *T() {
        auto t_mat = new Matrix<Type>(this->width, this->height);
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                t_mat->data[j][i] = this->Get(i, j);
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
        delete[](data);
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
        delete[](data);
        return m;
    }

    Type *operator[](int index) {
        return this->data[index];
    }

    Matrix<Type> *operator+(Type a) {
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i][j] = m->data[i][j] + a;
            }
        }
        return m;
    }

    Matrix<Type> *operator*(Type a) {
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i][j] = m->data[i][j] * a;
            }
        }
        return m;
    }


    Matrix<Type> *operator+(Matrix<Type> *a) {
        if (a->height == this->height && a->width == this->width) {
            auto data = Create2dArray<Type>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i][j] = this->Get(i, j) + a->Get(i, j);
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
        Matrix<Type> *row = nullptr;
        for (int i = 0; i < m->height; ++i) {
            row = m->Row(i);
            for (int j = 0; j < this->width; ++j) {
                m->data[i][j] = m->Get(i, j) + a->Get(0, j);
            }
            delete (row);
        }
        return m;
    }


    Matrix<Type> *operator-(Matrix<Type> *a) {

        if (a->height == this->height && a->width == this->width) {
            auto data = Create2dArray<Type>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i][j] = this->Get(i, j) - a->Get(i, j);
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
        Matrix<Type> *row = nullptr;
        for (int i = 0; i < m->height; ++i) {
            row = m->Row(i);
            for (int j = 0; j < this->width; ++j) {
                m->data[i][j] = m->Get(i, j) - a->Get(0, j);
            }
            delete (row);
        }
        return m;
    }

    Matrix<Type> *operator/(Matrix<Type> *a) {
        if(a->height == 1 && a->width == this->width){
            auto data = Create2dArray<double>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i][j] = this->Get(i, j) / a->Get(0, j);
                }
            }
            return new Matrix<double>(this->height, this->width, data);
        } else if(a->height == this->height && a->width == this->width){
            auto out = this->Copy();
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    Type val = a->Get(i,j);
                    if(val == 0){
                        printf("file: %s function: %s line: %d div zero.", __FILE__, __FUNCTION__,
                               __LINE__);
                        exit(-1);
                    }
                    out->data[i][j] = this->Get(i,j) / val;
                }
            }
            return out;
        }
        printf("file: %s function: %s line: %d dim not match: %d x %d --- %d x %d", __FILE__, __FUNCTION__,
               __LINE__, this->height, this->width, a->height, a->width);
        exit(-1);
    }

    Matrix<Type> *operator/(Type a) {
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i][j] = m->data[i][j] / a;
            }
        }
        return m;
    }

    Matrix<Type> *mat_pow(double e){
        auto out = this->Copy();
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                out->Set(i, j, pow(this->Get(i,j), e));
            }
        }
        return out;
    }

    //dot优化
    Matrix<Type> *Dot(Matrix<Type> *m) {
        if (this->width != m->height) {
            printf("file: %s function: %s line: %d dim not match: %d x %d --- %d x %d", __FILE__, __FUNCTION__,
                   __LINE__, this->height, this->width, m->height, m->width);
            exit(-1);
        }
        auto out = new Matrix<double>(this->height, m->width);
        for (int i = 0; i < this->height; ++i) {
            for (int k = 0; k < this->width; ++k) {
                if(this->data[i][k]!=0){
                    for (int j = 0; j < m->width; ++j) {
                        out->data[i][j] += this->data[i][k] * m->data[k][j];
                    }
                }
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
        auto data = Create2dArray<Type>(row, col);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[i][j] = (*all)[index++];
            }
        }
        delete (all);
        return new Matrix<Type>(row, col, data);
    }


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
        std::vector<Type> all;
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                all.push_back(this->Get(i, j));
            }
        }
        memcpy(line, all.data(), sizeof(Type) * this->height * this->height);
        return line;
    }

    Matrix<Type> *padding(int pad){
        int width = this->width + (2 * pad);
        int height = this->height + (2 * pad);
        auto *pad_mat = new Matrix<Type>(height, width);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if(i>=pad && i<height-pad && j>=pad && j<width-pad){
                    pad_mat->Set(i, j, this->Get(i-pad, j-pad));
                }
            }
        }
        return pad_mat;
    }

    Matrix<Type> *rot180(){
        auto rot = this->Copy();
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                rot->Set(rot->height-1-i, rot->width-1-j, this->Get(i, j));
            }
        }
        return rot;
    }


    //矩阵求和
    Type mat_sum(){
        Type sum = 0;
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                sum += this->Get(i, j);
            }
        }
        return sum;
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
