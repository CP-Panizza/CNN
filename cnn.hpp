#ifndef _CNN_HPP_
#define _CNN_HPP_


#include "layers.h"
#include "adam.hpp"


class Cnn {
public:
    Conv *conv;
    Pooling *pooling;
    ConvRelu *conv_relu;
    PoolingAffine *fc;
    Relu *relu;
    Affine *fc1;
    Softmax *soft_max;
    Adam *adam;

    double lr = 0.01;

    Cnn(int batch_size) {
        double std_init_whight = 0.01;
        int channel = 1;
        int img_w = 28;
        int img_h = 28;
        int filter_num = 50;
        int filter_size = 5;
        int stride = 1;
        int pad = 0;
        int hide_size = 50;
        conv = new Conv(std_init_whight, batch_size, channel, img_w, img_h, filter_num, filter_size, stride, pad);
        pooling = new Pooling(2, 2, conv->out_shape);
        conv_relu = new ConvRelu;
        fc = new PoolingAffine(pooling->out_shape, hide_size);
        relu = new Relu;
        fc1 = new Affine(fc->out_shape, 10);
        soft_max = new Softmax;
        this->adam = new Adam(this->lr);
    }


    void train(std::vector<std::vector<Matrix<double> *> > *x, Matrix<double> *la) {
        //前向传播
        auto conv_out = conv->forward(x);
        auto conv_relu_out = conv_relu->forward(conv_out);
        auto pooling_out = pooling->forward(conv_relu_out);
        auto fc_out = fc->forward(pooling_out);
        auto relu_out = relu->forward(fc_out);
        auto fc1_out = fc1->forward(relu_out);
        auto y = soft_max->forward(fc1_out, la);


        free_data(conv_out);
        free_data(conv_relu_out);
        free_data(pooling_out);
        delete (fc_out);
        delete (relu_out);
        delete (fc1_out);
        delete (y);


        //反向传播
        auto sm_dout = soft_max->backward();
        auto fc1_dout = fc1->backward(sm_dout);
        auto relu_dout = relu->backward(fc1_dout);
        auto fc_dout = fc->backword(relu_dout);
        auto pooling_dout = pooling->backword(fc_dout);
        auto conv_relu_dout = conv_relu->backword(pooling_dout);
        conv->backword(conv_relu_dout);

        delete (sm_dout);
        delete (fc1_dout);
        delete (relu_dout);
        free_data(fc_dout);
        free_data(pooling_dout);
        free_data(conv_relu_dout);


        //参数更新
        std::vector<std::string> keys = {"fc_w", "fc_b", "fc1_w", "fc1_b", "conv_w", "conv_b",};
        auto params = new std::map<string, Matrix<double> *>;
        params->insert(std::pair<string, Matrix<double> *>("fc_w", fc->W->Copy()));
        params->insert(std::pair<string, Matrix<double> *>("fc_b", fc->b->Copy()));
        params->insert(std::pair<string, Matrix<double> *>("fc1_w", fc1->W->Copy()));
        params->insert(std::pair<string, Matrix<double> *>("fc1_b", fc1->b->Copy()));
        params->insert(std::pair<string, Matrix<double> *>("conv_w", conv->W->Copy()));
        params->insert(std::pair<string, Matrix<double> *>("conv_b", conv->b->Copy()));

        auto grads = new std::map<string, Matrix<double> *>;
        grads->insert(std::pair<string, Matrix<double> *>("fc_w", fc->dW));
        grads->insert(std::pair<string, Matrix<double> *>("fc_b", fc->db));
        grads->insert(std::pair<string, Matrix<double> *>("fc1_w", fc1->dW));
        grads->insert(std::pair<string, Matrix<double> *>("fc1_b", fc1->db));
        grads->insert(std::pair<string, Matrix<double> *>("conv_w", conv->dW));
        grads->insert(std::pair<string, Matrix<double> *>("conv_b", conv->db));

        this->adam->update(params, grads);

        //参数设置到每一层
        delete (this->conv->W);
        this->conv->W = (*params)["conv_w"];
        delete (this->conv->b);
        this->conv->b = (*params)["conv_b"];


        delete (this->fc1->W);
        this->fc1->W = (*params)["fc1_w"];
        delete (this->fc1->b);
        this->fc1->b = (*params)["fc1_b"];


        delete (this->fc->W);
        this->fc->W = (*params)["fc_w"];
        delete (this->fc->b);
        this->fc->b = (*params)["fc_b"];
        delete (params);
        delete (grads);
    }

    Matrix<double> *predict(std::vector<std::vector<Matrix<double> *> > *x, Matrix<double> *la) {
        //前向传播
        auto conv_out = conv->forward(x);
//        std::cout << "conv_out:\n" << (*conv_out)[0][0];
        auto conv_relu_out = conv_relu->forward(conv_out);
//        std::cout << "conv_relu_out:\n" << (*conv_relu_out)[0][0];
        auto pooling_out = pooling->forward(conv_relu_out);
//        std::cout << "plooling_out:\n" << (*pooling_out)[0][0];
        auto fc_out = fc->forward(pooling_out);
//        std::cout << "fc_out:\n" << fc_out;
        auto relu_out = relu->forward(fc_out);
//        std::cout << "relu_out:\n" << relu_out;
        auto fc1_out = fc1->forward(relu_out);
//        std::cout << "fc1_out:\n" << fc1_out;
        auto y = soft_max->forward(fc1_out, la);
        free_data(conv_out);
        free_data(conv_relu_out);
        free_data(pooling_out);
        delete (fc_out);
        delete (relu_out);
        delete (fc1_out);

        return y;
    }

};


#endif //_CNN_HPP_