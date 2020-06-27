#include <iostream>
#include <cstdlib>
#include "matrix.hpp"
#include <ctime>
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



int main() {


    auto aa = new Matrix<double>{{1,2,3},{4,5,6},{9,0,0}};

    auto bb = aa->T();
    time_t s,e;
    s = clock();
    std::cout << aa->Dot(bb);
    e = clock();

    auto sec = (e - s) / 1000;
    std::cout << "Dot():" << sec <<std::endl;

    return 0;


    std::vector<std::vector<Matrix<double> *> > imgs;
    std::vector<int> labels;

    std::vector<std::vector<Matrix<double> *> > t_imgs;
    std::vector<int> t_labels;
//    load_faces_dataset(imgs, labels);

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
    int epoch = 100;
    int start;
    int end;
    auto cnn = new Cnn(batch_size);
#define TRAIN

#ifdef TEST
    if (!cnn->load_param("./param.json")) {
        exit(-1);
    }
#endif //TEST

//    system("Img2PGM2.exe -i test.jpg -o test.pgm");
//    Pgmer *pgmer = new Pgmer;
//    pgmer->ReadImg("./test.pgm");
//    auto data = pgmer->To2DMatrix()->operator/(255.0);
//    auto x = new std::vector<std::vector<Matrix<double> *>>{std::vector<Matrix<double> *>{data}};
//
//    auto test_la = new Matrix<double>(1, 2);
//    for (int j = 0; j < 1; ++j) {
//        test_la->Set(j, 1, 1);
//    }
//
//    auto pre = cnn->predict(x, test_la);
//
//    auto result = argmax(pre, "r");
//    std::cout << "predict:" << result;

#ifdef TRAIN

    Matrix<double> *la;
    for (int i = 0; i < epoch; ++i) {
        std::cout << "epoch:" << i << "\n";
        start = static_cast<int>(rand() % (imgs.size() - batch_size));
        end = start + batch_size;
        std::cout << "start:" << start << " end:" << end << "\n";
        std::vector<std::vector<Matrix < double> *> > x(imgs.begin() + start, imgs.begin() + end);
        std::vector<int> label(labels.begin() + start, labels.begin() + end);
        la = new Matrix<double>(batch_size, 10);
        for (int j = 0; j < batch_size; ++j) {
            la->Set(j, label[j], 1);
        }
        time_t s,e;
        s = clock();
        cnn->train(&x, la);
        e = clock();
        double sec = (e - s) / 1000.0;
        std::cout << "time:" << sec <<std::endl;
        delete (la);
    }

//    cnn->save_param("./param.json");

#endif //TRAIN
    //用测试集测试
    int test_num = 100;
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

    return 0;
}



