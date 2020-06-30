#include <iostream>
#include <cstdlib>
#include "matrix.hpp"
#include <ctime>
#include <zconf.h>
#include "utils.h"
#include "load_data.hpp"
#include "conv.hpp"
#include "conv_relu.hpp"
#include "pooling.hpp"
#include "pooling_affine.hpp"
#include "layers.h"
#include "cnn.hpp"
#include "adam.hpp"
#include "pgmer.hpp"
#include "deep_cnn.hpp"

void train_faces() {
    std::vector<std::vector<Matrix<double> *> > imgs;
    std::vector<int> labels;
    load_faces_dataset("data/faces_dataset/faces_and_nofaces.txt", imgs, labels);
//    load_faces_dataset("data/faces_dataset/img_db.txt", imgs, labels);
    srand(static_cast<unsigned int>(time(nullptr)));
    int batch_size = 28;
    int channel = 1;
    int img_w = 40;
    int img_h = 40;
    int start;
    int end;

    std::vector<int> input_shape{batch_size, channel, img_h, img_w};
    auto deep_cnn = new DeepCnn(input_shape);

    int epoch = 500;

    Matrix<double> *la;
    for (int i = 0; i < epoch; ++i) {
        start = static_cast<int>(rand() % (imgs.size() - batch_size));
        end = start + batch_size;
        std::vector<std::vector<Matrix<double> *> > x(imgs.begin() + start, imgs.begin() + end);
        std::vector<int> label(labels.begin() + start, labels.begin() + end);
        la = new Matrix<double>(batch_size, 2);
        for (int j = 0; j < batch_size; ++j) {
            la->Set(j, label[j], 1);
        }
        time_t s, e;
        s = clock();
        double loss = deep_cnn->train(&x, la);
        e = clock();
        double sec = (e - s) / 1000.0;
        char buff[50];
        int n = sprintf(buff, "los:%.2lf", loss);
        buff[n] = '\0';
        progress_bar(i, epoch, sec, buff);
        delete (la);
    }

//    cnn->save_param("./faces_param.json");

//    int test_num = 200;
//    int right_times = 0;
//    for (int k = 0; k < test_num; ++k) {
//        start = static_cast<int>(rand() % (imgs.size() - 1));
//        end = start + 1;
//
//        std::vector<std::vector<Matrix<double> *> > test_x(imgs.begin() + start, imgs.begin() + end);
//        std::vector<int> test_label(labels.begin() + start, labels.begin() + end);
//        auto test_la = new Matrix<double>(1, 2);
//        for (int j = 0; j < 1; ++j) {
//            test_la->Set(j, test_label[j], 1);
//        }
//        std::cout << "\nNo:" << k << "\n";
//        std::cout << "start:" << start << " end:" << end << "\n";
//        auto pre = cnn->predict(&test_x, test_la);
//        auto result = argmax(pre, "r");
//        if (result->Get(0, 0) == test_label[0]) {
//            right_times++;
//        }
//        delete (test_la);
//        delete (pre);
//        delete (result);
//    }
//    std::cout << "right_p:" << (double(right_times) / test_num) * 100.0 << "%";
}


//minats手写数字训练
void train() {
    std::vector<std::vector<Matrix<double> *> > imgs;
    std::vector<int> labels;

    std::vector<std::vector<Matrix<double> *> > t_imgs;
    std::vector<int> t_labels;

    Load_data(
            R"(data/img_label/train-images.idx3-ubyte)",
            R"(data/img_label/train-labels.idx1-ubyte)",
            imgs, labels);

    Load_data(
            R"(data/img_label/t10k-images.idx3-ubyte)",
            R"(data/img_label/t10k-labels.idx1-ubyte)",
            t_imgs, t_labels);

    srand(static_cast<unsigned int>(time(nullptr)));
    int batch_size = 30;
    int start;
    int end;
    int channel = 1;
    int img_w = 28;
    int img_h = 28;
    int filter_num = 50;
    int filter_size = 5;
    int stride = 1;
    int pad = 2;
    std::vector<int> input_shape{batch_size, channel, img_h, img_w};
    auto cnn = new Cnn(filter_num, filter_size, stride, pad, input_shape);

    int epoch = 1000;
    Matrix<double> *la;
    for (int i = 0; i < epoch; ++i) {
        start = static_cast<int>(rand() % (imgs.size() - batch_size));
        end = start + batch_size;
        std::vector<std::vector<Matrix<double> *> > x(imgs.begin() + start, imgs.begin() + end);
        std::vector<int> label(labels.begin() + start, labels.begin() + end);
        la = new Matrix<double>(batch_size, 10);
        for (int j = 0; j < batch_size; ++j) {
            la->Set(j, label[j], 1);
        }
        time_t s, e;
        s = clock();
        double loss = cnn->train(&x, la);
        e = clock();
        double sec = (e - s) / 1000.0;
        char buff[50];
        int n = sprintf(buff, "loss %.2lf", loss);
        buff[n] = '\0';
        progress_bar(i, epoch, sec, buff);
        delete (la);
    }

    cnn->save_param("./T-param.json");
}


void test() {
    std::vector<std::vector<Matrix<double> *> > imgs;
    std::vector<int> labels;

    std::vector<std::vector<Matrix<double> *> > t_imgs;
    std::vector<int> t_labels;

    Load_data(
            R"(data/img_label/train-images.idx3-ubyte)",
            R"(data/img_label/train-labels.idx1-ubyte)",
            imgs, labels);

    Load_data(
            R"(data/img_label/t10k-images.idx3-ubyte)",
            R"(data/img_label/t10k-labels.idx1-ubyte)",
            t_imgs, t_labels);

    srand(static_cast<unsigned int>(time(nullptr)));
    int batch_size = 30;
    int start;
    int end;
    int channel = 1;
    int img_w = 28;
    int img_h = 28;
    int filter_num = 50;
    int filter_size = 5;
    int stride = 1;
    int pad = 2;
    std::vector<int> input_shape{batch_size, channel, img_h, img_w};
    auto cnn = new Cnn(filter_num, filter_size, stride, pad, input_shape);

    int test_num = 200;
    int right_times = 0;
    for (int k = 0; k < test_num; ++k) {
        start = static_cast<int>(rand() % (imgs.size() - 1));
        end = start + 1;

        std::vector<std::vector<Matrix<double> *> > test_x(imgs.begin() + start, imgs.begin() + end);
        std::vector<int> test_label(labels.begin() + start, labels.begin() + end);
        auto test_la = new Matrix<double>(1, 10);
        for (int j = 0; j < 1; ++j) {
            test_la->Set(j, test_label[j], 1);
        }
        std::cout << "\nNo:" << k << "\n";
        std::cout << "start:" << start << " end:" << end << "\n";
        auto pre = cnn->predict(&test_x, test_la);
        auto result = argmax(pre, "r");
        if (result->Get(0, 0) == test_label[0]) {
            right_times++;
        }
        delete (test_la);
        delete (pre);
        delete (result);
    }
    std::cout << "right_p:" << (double(right_times) / test_num) * 100.0 << "%";

}


void test_jpg() {
    system("Img2PGM2.exe -i test.jpg -o test.pgm");
    Pgmer *pgmer = new Pgmer;
    pgmer->ReadImg("./test.pgm");
    auto data = pgmer->To2DMatrix()->operator/(255.0);
    auto x = new std::vector<std::vector<Matrix<double> *>>
            {std::vector<Matrix<double> *>{data}};

    auto test_la = new Matrix<double>(1, 2);
    for (int j = 0; j < 1; ++j) {
        test_la->Set(j, 1, 1);
    }
    int batch_size = 30;
    int start;
    int end;
    int channel = 1;
    int img_w = 40;
    int img_h = 40;
    int filter_num = 50;
    int filter_size = 5;
    int stride = 1;
    int pad = 2;
    std::vector<int> input_shape{batch_size, channel, img_h, img_w};
    auto cnn = new Cnn(filter_num, filter_size, stride, pad, input_shape);
    cnn->load_param("./param.json");
    auto pre = cnn->predict(x, test_la);

    auto result = argmax(pre, "r");
    std::cout << "predict:" << result;
}


int main() {
    train_faces();

    return 0;
}



