//
// Created by Administrator on 2020/6/3.
//


#include <ctime>
#include "utils.h"
#include <cmath>
#include <stdio.h>
#include <sstream>

Matrix<double> *
im2col(std::vector<std::vector<Matrix<double> *>> *input_data, int filter_h, int filter_w, int stride, int pad) {
    int N = static_cast<int>(input_data->size()), C = static_cast<int>(((*input_data)[0]).size()), H = ((*input_data)[0])[0]->height, W = ((*input_data)[0])[0]->width;
//    int out_h = (H + 2 * pad - filter_h) / stride + 1;
//    int out_w = (W + 2 * pad - filter_w) / stride + 1;

    auto input_data_copy = new std::vector<std::vector<Matrix<double> *>>;
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
        for (i = 0; i <= padded_img_h - filter_h; i += stride) {
            for (j = 0; j <= padded_img_w - filter_w; j += stride) {
                for (n = 0; n < vec.size(); n++) {
                    for (k = 0; k < filter_h; ++k) {
                        for (m = 0; m < filter_w; ++m) {
                            line.push_back(vec[n]->Get(i + k, j + m));
                        }
                    }
                }
            }
        }
        result.push_back(line);
    }

    free_data(input_data_copy);

    int width = C * filter_h * filter_w;
    int height = N * (result[0].size() / width);

    double *data = CreateArray<double>(height, width);

    int row = 0, col = 0;
    int length;
    for (auto &v: result) {
        length = static_cast<int>(v.size());
        if (row != 0) {
            row++;
        }
        for (i = 0; i < length; ++i) {
            if (i != 0 && i % width == 0) {
                row++;
            }
            data[row * width + col++] = v[i];
            if (col == width) {
                col = 0;
            }
        }
    }
    return new Matrix<double>(height, width, data);
}


Matrix<int> *argmax(Matrix<double> *mat, std::string dim) {
    int width = 0;
    int *data = nullptr;
    if (dim == "c") {
        width = mat->width;
        data = CreateArray<int>(1, width);
        Matrix<double> *col = nullptr;
        for (int i = 0; i < mat->width; ++i) {
            col = mat->Col(i);
            double max = col->Get(0, 0);
            int max_index = 0;
            for (int j = 0; j < col->height; ++j) {
                if (col->Get(j, 0) > max) {
                    max = col->Get(j, 0);
                    max_index = j;
                }
            }
            data[i] = max_index;
            delete (col);
        }
    } else if (dim == "r") {
        width = mat->height;
        data = CreateArray<int>(1, width);
        Matrix<double> *row = nullptr;
        for (int i = 0; i < mat->height; ++i) {
            row = mat->Row(i);
            double max = row->Get(0, 0);
            int max_index = 0;
            for (int j = 0; j < row->width; ++j) {
                if (row->Get(0, j) > max) {
                    max = row->Get(0, j);
                    max_index = j;
                }
            }
            data[i] = max_index;
            delete (row);
        }
    }
    return new Matrix<int>(1, width, data);
}

Matrix<double> *max(Matrix<double> *mat, std::string dim) {
    int width = 0;
    double *data = nullptr;
    if (dim == "c") {
        width = mat->width;
        data = CreateArray<double>(1, width);
        Matrix<double> *col = nullptr;
        for (int i = 0; i < mat->width; ++i) {
            col = mat->Col(i);
            double max = col->Get(0, 0);
            for (int j = 0; j < col->height; ++j) {
                if (col->Get(j, 0) > max) {
                    max = col->Get(j, 0);
                }
            }
            data[i] = max;
            delete (col);
        }
    } else if (dim == "r") {
        width = mat->height;
        data = CreateArray<double>(1, width);
        Matrix<double> *row = nullptr;
        for (int i = 0; i < mat->height; ++i) {
            row = mat->Row(i);
            double max = row->Get(0, 0);
            for (int j = 0; j < row->width; ++j) {
                if (row->Get(0, j) > max) {
                    max = row->Get(0, j);
                }
            }
            data[i] = max;
            delete (row);
        }
    }
    return new Matrix<double>(1, width, data);
}

Matrix<double> *sum(Matrix<double> *mat, std::string dim) {
    int width = 0;
    double *data = nullptr;
    if (dim == "c") {
        width = mat->width;
        data = CreateArray<double>(1, width);
        Matrix<double> *col = nullptr;
        for (int i = 0; i < mat->width; ++i) {
            col = mat->Col(i);
            double sum = 0;
            for (int j = 0; j < col->height; ++j) {
                sum += col->Get(j, 0);
            }
            data[i] = sum;
            delete (col);
        }
    } else if (dim == "r") {
        width = mat->height;
        data = CreateArray<double>(1, width);
        Matrix<double> *row = nullptr;
        for (int i = 0; i < mat->height; ++i) {
            row = mat->Row(i);
            double sum = 0;
            for (int j = 0; j < row->width; ++j) {
                sum += row->Get(0, j);
            }
            data[i] = sum;
            delete (row);
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


Matrix<double> *rand_matrix_n(int row, int col) {
    auto data = new Matrix<double>(row, col);
    double min = 0.0;
    double max = 0.0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            double val = gauss_rand();
            max = val > max ? val : max;
            min = val < min ? val : min;
            data->Set(i, j, val);
        }
    }
    if (min < 0) {
        auto than_zero = data->operator+(fabs(min));
        auto out = than_zero->operator/(max+fabs(min));
        delete(than_zero);
        return out;
    }
    auto out = data->operator/(max);
    delete(data);
    return out;
}

/**
 * 生成随机正态分布矩阵
 * @param row
 * @param col
 * @return
 */
Matrix<double> *rand_matrix(int row, int col) {
    auto data = new Matrix<double>(row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data->Set(i, j, gauss_rand());
        }
    }
    return data;
}

Matrix<double> *mat_exp(Matrix<double> *x) {
    auto data = CreateArray<double>(x->height, x->width);
    double v;
    for (int i = 0; i < x->height; ++i) {
        for (int j = 0; j < x->width; ++j) {
            v = x->Get(i, j);
            data[i * x->width + j] = exp(v);
        }
    }
    return new Matrix<double>(x->height, x->width, data);
}

Matrix<double> *mat_log(Matrix<double> *x) {
    auto data = CreateArray<double>(x->height, x->width);
    double v;
    for (int i = 0; i < x->height; ++i) {
        for (int j = 0; j < x->width; ++j) {
            v = x->Get(i, j);
            data[i * x->width + j] = log(v);
        }
    }
    return new Matrix<double>(x->height, x->width, data);
}


Matrix<double> *padding(Matrix<double> *src, int pad) {
    int width = src->width + (2 * pad);
    int height = src->height + (2 * pad);
    auto *pad_mat = new Matrix<double>(height, width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i >= pad && i < height - pad && j >= pad && j < width - pad) {
                pad_mat->Set(i, j, src->Get(i - pad, j - pad));
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
double gauss_rand() {
    static double V1, V2, S;
    static int phase = 0;
    double X;
    if (phase == 0) {
        do {
            double U1 = (double) rand() / RAND_MAX;
            double U2 = (double) rand() / RAND_MAX;
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while (S >= 1 || S == 0);
        X = V1 * sqrt(-2 * log(S) / S);
    } else {
        X = V2 * sqrt(-2 * log(S) / S);
    }
    phase = 1 - phase;

    return X;
}

std::string read_file(std::string file) {
    std::ifstream fin(file);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string str(buffer.str());
    return str;
}

/**
 * 单张图片的还原
 * @param col 需要变形回去的矩阵
 * @param src_w 原始矩阵宽度
 * @param src_h 原始矩阵高度
 * @param filter_size 卷积核边长
 * @param stride 卷积滑动步长
 * @return
 */
Matrix<double> *col2im(Matrix<double> *col, int src_w, int src_h, int filter_size, int stride) {
    auto origin = new Matrix<double>(src_h, src_w);
    auto mask = new Matrix<double>(src_h, src_w);
    int num_row = 0; //col矩阵的第几行
    Matrix<double> *row = nullptr;
    for (int i = 0; i <= origin->height - filter_size; i += stride) {
        for (int j = 0; j <= origin->width - filter_size; j += stride) {
            row = col->Row(num_row++);
            int idx = 0;
            for (int m = i; m < i + filter_size; ++m) {
                for (int n = j; n < j + filter_size; ++n) {
                    if (mask->Get(m, n) == 0) {
                        origin->Set(m, n, row->Get(0, idx++));
                    } else {
                        idx++;
                    }
                }
            }
            delete (row);
        }
    }
    delete (mask);
    return origin;
}


std::string &replace_all(std::string &str, const std::string &old_value, const std::string &new_value) {
    while (true) {
        std::string::size_type pos(0);
        if ((pos = str.find(old_value)) != std::string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else { break; }
    }
    return str;
}

/**
 *
 * @param per 当前第几个
 * @param totle 总共多少个
 */
//const char *ponit[]={"\x20\x20", "\xA8\x87", "\xA8\x86", "\xA8\x84", "\xA8\x83", "\xA8\x80"};//  ▏▎▍▊█

void progress_bar(int per, int totle, double time, char *info) {
    char bar[51];
    int done = static_cast<int>((double(per) / double(totle)) * 50);
    int i;
    for (i = 0; i < done; i++) {
        bar[i] = '#';
    }
    bar[i] = 0;
    printf("[%-50s][%.1lf%%][%d/%d][time:%.2lf][%s]\r", bar, double(per) / double(totle) * 100.0, per, totle, time, info);
    fflush(stdout);
}

double cubic_coeff(double x) {
    x = (x > 0) ? x : -x;
    if (x < 1) {
        return 1 - 2 * x * x + x * x * x;
    } else if (x < 2) {
        return 4 - 8 * x + 5 * x * x - x * x * x;
    }
    return 0;
}

/**
 * 对double四舍五入
 * @return
 */
int _round_(double a){
    int b;
    if(a>0){
        b = static_cast<int>((a * 2 + 1) / 2);
    }else {
        b = static_cast<int>((a * 2 - 1) / 2);
    }
    return b;
}

/**
 * 图像scale变换
 * @param src 原图像
 * @param sigma scale变换率
 * @return
 */
Matrix<double> *ReSize(Matrix<double> *src, double sigma) {
    double min_val = 0.0;
    double max_val = 0.0;
    auto dest = new Matrix<double>(static_cast<int>(src->height * sigma), static_cast<int>(src->width * sigma));

    for (int i = 0; i < src->height; ++i) {
        for (int j = 0; j < src->width; ++j) {

            double col = j * sigma;
            double row = i * sigma;
            int dest_x = static_cast<int>(col); //i
            int dest_y = static_cast<int>(row); //j

            double col_ = (j - 1) * sigma;
            double row_ = (i - 1) * sigma;
            int dest_x_ = static_cast<int>(col_); //i
            int dest_y_ = static_cast<int>(row_); //j

            //处理超出边界的像素
            if ((j - 1) < 0 || (i - 1) < 0 || (i + 2) > (src->height - 1) ||
                (j + 2) > (src->width - 1)) {
                continue;
            }

            double values[4][4];
            for (int r = i - 1, s = 0; r <= i + 2; ++r, s++) {
                for (int c = j - 1, t = 0; c <= j + 2; ++c, t++) {
                    values[s][t] = src->Get(r, c);
                }
            }

            double u = col - dest_x;
            double v = row - dest_y;
            double A[4], C[4];

            for (int distance = 1, s = 0; distance >= -2; distance--, s++) {
                A[s] = cubic_coeff(u + distance);
                C[s] = cubic_coeff(v + distance);
            }

            double dfCubicValue = 0.0;
            for (int s = 0; s < 4; s++) {
                for (int t = 0; t < 4; t++) {
                    dfCubicValue += values[s][t] * A[t] * C[s];
                }
            }

            for (int m = dest_y_; m < dest_y; ++m) {
                for (int n = dest_x_; n < dest_x; ++n) {
                    dest->Set(m, n, dfCubicValue);
                }
            }

            if (dfCubicValue < min_val) {
                min_val = dfCubicValue;
            }

            if (dfCubicValue > max_val) {
                max_val = dfCubicValue;
            }
        }
    }


    if (min_val < 0) {
        auto then_zero = dest->operator+(fabs(min_val)); //大于零
        auto one = then_zero->operator/(max_val); //归一化
        auto out = one->operator*(255.0); //复原亮度
        delete (then_zero);
        delete (dest);
        delete (one);
        return out;
    }


    auto one = dest->operator/(max_val); //归一化
    auto out = one->operator*(255.0); //复原亮度
    delete (dest);
    delete (one);
    return out;
}


double cross_entropy_error(Matrix<double> *y, Matrix<double> *t){
//    batch_size = y.shape[0]
//    print(np.log(y+delta))
//    print(t*np.log(y+delta))
//    return -np.sum(t*np.log(y+1e-7))/batch_size
    int batch_size = y->height;
    auto _y = y->operator+(1e-7);
    auto _matlog = mat_log(_y);
    auto mul = t->operator*(_matlog);
    double out = -1 * mul->mat_sum() / batch_size;
    delete(_y);
    delete(_matlog);
    delete(mul);
    return out;
}

/**
 * 建立图像金字塔，并切分成20x20大小的适合网络输入的图像
 * @param src 原始输入图像，不固定宽高
 * @param min 金字塔图像中最小宽度或高度
 * @param max 金字塔图像中最大宽度或高度
 * @param sigmas 每个剪切好的图像对应的sigma
 * @param imgs 剪切好的图像
 * @param sub_img_h_w截取图像的宽度和高度
 * @param stride 截取图片步长
 */
void Build_img(int sub_img_h_w, int stride, Matrix<double> *src, double min, double max, std::vector<double> &sigmas, std::vector<Matrix<double>*> &imgs, std::vector<Point> &points){
    int min_length = src->width < src->height? src->width:src->height;  //取输入图像的宽和高中的小的值
    double sigma1 = min / double(min_length);//最小sigma
    double sigma2 = max / double(min_length);//最大sigma

    int step = _round_(sigma2 / sigma1); //图像从最小到最大总共缩放的次数

    double step_sigma = (sigma2 - sigma1) / double(step);  //每一次放大sigma的增量

    double init_sigma = sigma1;

    for (int i = 0; i < step; ++i) {
        auto _scale_img = ReSize(src, init_sigma);
        for (int m = 0; m <= _scale_img->height - sub_img_h_w; m+=stride) {
            for (int n = 0; n <= _scale_img->width - sub_img_h_w ; n+=stride) {
                auto sub_img = _scale_img->SubMat(m, n, sub_img_h_w,sub_img_h_w);
                imgs.push_back(sub_img->operator/(255.0));
                sigmas.push_back(init_sigma);
                points.push_back(Point{m,n});
                delete(sub_img);
            }
        }
        init_sigma += step_sigma;
    }
}



