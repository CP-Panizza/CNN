#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <fstream>
double **Create2dArray(int row, int col){
    double **array = new double *[row];
    for (int i = 0; i < row; ++i) {
        array[i] = new double[col];
    }
    return array;
}

class Matrix;
std::ostream & operator<<( std::ostream & os, Matrix *matrix);


class Matrix {
public:
    int width;
    int height;
    double **data = nullptr;

    friend std::ostream & operator<<( std::ostream & os, Matrix *matrix);
    Matrix() {}

    ~Matrix(){
        for (int i = 0; i < this->height; ++i) {
            delete[](this->data[i]);
        }
        delete[](this->data);
    }

    Matrix(const std::initializer_list<std::initializer_list<double >>& inList){
        this->height = static_cast<int>(inList.size());
        this->width = static_cast<int>(inList.begin()->size());
        this->data = Create2dArray(this->height, this->width);
        int i = 0, j = 0;
        for (auto &x :inList) {
            for(auto &y: x){
                this->data[i][j] = y;
                j++;
            }
            j = 0;
            i++;
        }
    }

    Matrix(const std::initializer_list<double >& inList){
        this->width = static_cast<int>(inList.size());
        this->height = 1;
        this->data = Create2dArray(this->height, this->width);
        int i = 0;
        for (auto &x :inList) {
            this->data[0][i] = x;
            i++;
        }
    }

    Matrix(int h, int w, double *_data) {
        this->data = Create2dArray(h, w);
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                this->data[j][i] = _data[w * j + i];
            }
        }

        this->width = w;
        this->height = h;
    }


    Matrix *Copy(){
        auto t = new Matrix;
        t->width = this->width;
        t->height = this->height;
        t->data = Create2dArray(this->height, this->width);
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

    bool Set(int row, int col, double val) {
        if (col > this->width || row > this->height) return false;
        this->data[row][col] = val;
        return true;
    }

    double Get(int row, int col) {
        if (col > this->width || row > this->height) return -1;
        return this->data[row][col];
    }

    Matrix *T(){
        auto t = Create2dArray(this->width, this->height);
        auto t_mat = new Matrix;
        t_mat->height = this->width;
        t_mat->width = this->height;
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                t[j][i] = this->Get(i, j);
            }
        }
        t_mat->data = t;
        return t_mat;
    }

    Matrix *Row(int index){
        if (index >= this->height || index < 0){
            printf("err: index %d beyond\n", index);
            exit(-1);
        }
        auto data = new double[this->width];

        for (int i = 0; i < this->width; ++i) {
            data[i] = this->Get(index, i);
        }
        Matrix *m = new Matrix(1, this->width, data);

        return m;
    }


    Matrix *Col(int index){
        if (index >= this->width || index < 0){
            printf("err: index %d beyond\n", index);
            exit(-1);
        }
        auto data = new double[this->height];
        for (int i = 0; i < this->height; ++i) {
            data[i] = this->Get(i, index);
        }
        Matrix *m = new Matrix(this->height, 1 , data);
        return m;
    }

    double *operator[](int index){
        return this->data[index];
    }

    Matrix *operator+(double a){
        Matrix *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i][j]  = m->data[i][j] + a;
            }
        }
        return m;
    }


    Matrix *operator+(Matrix *a){
        if(a->height != 1 || a->width != this->width){
            printf("dim not match: %d x %d --- %d x %d", this->height,this->width, a->height, a->width);
            exit(-1);
        }
        Matrix *m = this->Copy();
        Matrix *row = nullptr;
        for (int i = 0; i < m->height; ++i) {
            row = m->Row(i);
            for (int j = 0; j < this->width; ++j) {
                m->data[i][j] = m->Get(i, j) + row->Get(1, j);
            }
            delete(row);
        }
        return m;
    }


    Matrix *Dot(Matrix *m){
        if(this->width != m->height){
            printf("dim not match: %d x %d --- %d x %d", this->height,this->width, m->height, m->width);
            exit(-1);
        }
        auto data = Create2dArray(this->height, m->width);
        double sum = 0.0;
        Matrix *row = nullptr;
        Matrix * col = nullptr;
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                row = this->Row(i);
                col = m->Col(j)->T();
                for (int k = 0; k < row->width; ++k) {
                    sum += row->Get(0, k) * col->Get(0, k);
                }
                data[i][j] = sum;
                sum = 0.0;
                delete(row);
                delete(col);
            }
        }
        Matrix *result = new Matrix;
        result->width = m->width;
        result->height = this->height;
        result->data = data;
    }


};


std::ostream & operator<<( std::ostream & os, Matrix *matrix)
{
    for (int i = 0; i < matrix->height; ++i) {
        for (int j = 0; j < matrix->width; ++j) {
            os << matrix->Get(i, j) << ", ";
        }
        os << "\n";
    }
    return os;
}


#endif