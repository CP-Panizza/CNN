#ifndef _DEEP_CNN_
#define _DEEP_CNN_

#include "layers.h"

class DeepCnn {
public:
    Conv *conv1;
    ConvRelu *relu2;
    Conv *conv3;
    ConvRelu *relu4;
    Pooling *pooling5;

    Conv *conv6;
    ConvRelu *relu7;
    Conv *conv8;
    ConvRelu *relu9;
    Pooling *pooling10;

    Conv *conv11;
    ConvRelu *relu12;
    Conv *conv13;
    ConvRelu *relu14;
    Pooling *pooling15;

    PoolingAffine *poolingAffine16;
    Relu *relu17;
    Dropout *dropout18;
    Affine *affine19;
    Dropout *dropout20;
    Softmax *softmax21;

    Adam *adam;
    double lr = 0.001;

    DeepCnn(std::vector<int> &input_shape) {
        int filter_size = 5;
        int stride = 1;
        int pad = 2;
        int hidden_size = 50;

        /**
         * He方式初始化权重参数
         */
        Matrix<double> pre_node_nums = {1 * 3 * 3,
                                        16 * 3 * 3,
                                        16 * 3 * 3,
                                        32 * 3 * 3,
                                        32 * 3 * 3,
                                        64 * 3 * 3,
                                        64 * 4 * 4,
                                        double(hidden_size)};
        for (int i = 0; i < pre_node_nums.width; ++i) {
            pre_node_nums.Set(0, i, 2.0 / (pre_node_nums.Get(0, i) + 1e-7));
        }
        auto wight_init_scales = pre_node_nums.mat_pow(0.5);

        conv1 = new Conv(wight_init_scales->Get(0, 0), 16, filter_size, stride, pad, input_shape);
        relu2 = new ConvRelu;
        conv3 = new Conv(wight_init_scales->Get(0, 1), 16, filter_size, stride, pad, conv1->out_shape);
        relu4 = new ConvRelu;
        pooling5 = new Pooling(2, 2, conv3->out_shape);

        conv6 = new Conv(wight_init_scales->Get(0, 2), 32, filter_size, stride, pad, pooling5->out_shape);
        relu7 = new ConvRelu;
        conv8 = new Conv(wight_init_scales->Get(0, 3), 32, filter_size, stride, pad, conv6->out_shape);
        relu9 = new ConvRelu;
        pooling10 = new Pooling(2, 2, conv8->out_shape);

        conv11 = new Conv(wight_init_scales->Get(0, 4), 64, filter_size, stride, pad, pooling10->out_shape);
        relu12 = new ConvRelu;
        conv13 = new Conv(wight_init_scales->Get(0, 5), 64, filter_size, stride, pad, conv11->out_shape);
        relu14 = new ConvRelu;
        pooling15 = new Pooling(2, 2, conv13->out_shape);

        poolingAffine16 = new PoolingAffine(wight_init_scales->Get(0, 6), pooling15->out_shape, hidden_size);
        relu17 = new Relu;
        dropout18 = new Dropout;
        affine19 = new Affine(wight_init_scales->Get(0, 7), poolingAffine16->out_shape, 2);
        dropout20 = new Dropout;
        softmax21 = new Softmax;
        adam = new Adam(this->lr);
    }

    double train(std::vector<std::vector<Matrix<double> *> > *x, Matrix<double> *la) {
        //forward
        auto conv1_out = conv1->forward(x);
        auto relu2_out = relu2->forward(conv1_out);
        auto conv3_out = conv3->forward(relu2_out);
        auto relu4_out = relu4->forward(conv3_out);
        auto pooling5_out = pooling5->forward(relu4_out);

        auto conv6_out = conv6->forward(pooling5_out);
        auto relu7_out = relu7->forward(conv6_out);
        auto conv8_out = conv8->forward(relu7_out);
        auto relu9_out = relu9->forward(conv8_out);
        auto pooling10_out = pooling10->forward(relu9_out);

        auto conv11_out = conv11->forward(pooling10_out);
        auto relu12_out = relu12->forward(conv11_out);
        auto conv13_out = conv13->forward(relu12_out);
        auto relu14_out = relu14->forward(conv13_out);
        auto pooling15_out = pooling15->forward(relu14_out);

        auto poolingAff16_out = poolingAffine16->forward(pooling15_out);
        auto relu17_out = relu17->forward(poolingAff16_out);
        auto dropout18_out = dropout18->forword(relu17_out);
        auto affine19_out = affine19->forward(dropout18_out);
        auto dropout20_out = dropout20->forword(affine19_out);
        auto loss = softmax21->forward(dropout20_out, la);

        free_data(conv1_out);
        free_data(relu2_out);
        free_data(conv3_out);
        free_data(relu4_out);
        free_data(pooling5_out);
        free_data(conv6_out);
        free_data(relu7_out);
        free_data(conv8_out);
        free_data(relu9_out);
        free_data(pooling10_out);
        free_data(conv11_out);
        free_data(relu12_out);
        free_data(conv13_out);
        free_data(relu14_out);
        free_data(pooling15_out);
        delete (poolingAff16_out);
        delete (relu17_out);
        delete (dropout18_out);
        delete (affine19_out);
        delete (dropout20_out);



        //backward
        auto softmax21_dout = softmax21->backward();
        auto dropout20_dout = dropout20->backward(softmax21_dout);
        auto affine19_dout = affine19->backward(dropout20_dout);
        auto dropout18_dout = dropout18->backward(affine19_dout);
        auto relu17_dout = relu17->backward(dropout18_dout);
        auto poolingAff16_dout = poolingAffine16->backword(relu17_dout);
        auto pooling15_dout = pooling15->backword(poolingAff16_dout);
        auto relu14_dout = relu14->backword(pooling15_dout);
        auto conv13_dout = conv13->backword(relu14_dout);
        auto relu12_dout = relu12->backword(conv13_dout);
        auto conv11_dout = conv11->backword(relu12_dout);
        auto pooling10_dout = pooling10->backword(conv11_dout);
        auto relu9_dout = relu9->backword(pooling10_dout);
        auto conv8_dout = conv8->backword(relu9_dout);
        auto relu7_dout = relu7->backword(conv8_dout);
        auto conv6_dout = conv6->backword(relu7_dout);
        auto pooling5_dout = pooling5->backword(conv6_dout);
        auto relu4_dout = relu4->backword(pooling5_dout);
        auto conv3_dout = conv3->backword(relu4_dout);
        auto relu2_dout = relu2->backword(conv3_dout);
        auto conv1_dout = conv1->backword(relu2_dout);


        free_data(conv1_dout);
        free_data(relu2_dout);
        free_data(conv3_dout);
        free_data(relu4_dout);
        free_data(pooling5_dout);
        free_data(conv6_dout);
        free_data(relu7_dout);
        free_data(conv8_dout);
        free_data(relu9_dout);
        free_data(pooling10_dout);
        free_data(conv11_dout);
        free_data(relu12_dout);
        free_data(conv13_dout);
        free_data(relu14_dout);
        free_data(pooling15_dout);
        delete (poolingAff16_dout);
        delete (relu17_dout);
        delete (dropout18_dout);
        delete (affine19_dout);
        delete (dropout20_dout);

        auto params = new std::map<std::string, Matrix<double> *>;
        params->insert(std::pair<std::string, Matrix<double> *>("conv1_w", conv1->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv1_b", conv1->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv3_w", conv3->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv3_b", conv3->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv6_w", conv6->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv6_b", conv6->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv8_w", conv8->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv8_b", conv8->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv11_w", conv11->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv11_b", conv11->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv13_w", conv13->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv13_b", conv13->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("fc16_w", poolingAffine16->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("fc16_b", poolingAffine16->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("fc19_w", affine19->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("fc19_b", affine19->b->Copy()));

        auto grads = new std::map<std::string, Matrix<double> *>;
        grads->insert(std::pair<std::string, Matrix<double> *>("conv1_w", conv1->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv1_b", conv1->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv3_w", conv3->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv3_b", conv3->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv6_w", conv6->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv6_b", conv6->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv8_w", conv8->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv8_b", conv8->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv11_w", conv11->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv11_b", conv11->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv13_w", conv13->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv13_b", conv13->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("fc16_w", poolingAffine16->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("fc16_b", poolingAffine16->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("fc19_w", affine19->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("fc19_b", affine19->db));

        this->adam->update(params, grads);


        delete (this->conv1->W);
        this->conv1->W = (*params)["conv1_w"];
        delete (this->conv1->b);
        this->conv1->b = (*params)["conv1_b"];


        delete (this->conv3->W);
        this->conv3->W = (*params)["conv3_w"];
        delete (this->conv3->b);
        this->conv3->b = (*params)["conv3_b"];


        delete (this->conv6->W);
        this->conv6->W = (*params)["conv6_w"];
        delete (this->conv6->b);
        this->conv6->b = (*params)["conv6_b"];


        delete (this->conv8->W);
        this->conv8->W = (*params)["conv8_w"];
        delete (this->conv8->b);
        this->conv8->b = (*params)["conv8_b"];


        delete (this->conv11->W);
        this->conv11->W = (*params)["conv11_w"];
        delete (this->conv11->b);
        this->conv11->b = (*params)["conv11_b"];


        delete (this->conv13->W);
        this->conv13->W = (*params)["conv13_w"];
        delete (this->conv13->b);
        this->conv13->b = (*params)["conv13_b"];


        delete (this->poolingAffine16->W);
        this->poolingAffine16->W = (*params)["fc16_w"];
        delete (this->poolingAffine16->b);
        this->poolingAffine16->b = (*params)["fc16_b"];


        delete (this->affine19->W);
        this->affine19->W = (*params)["fc19_w"];
        delete (this->affine19->b);
        this->affine19->b = (*params)["fc19_b"];

        delete (params);
        delete (grads);

        return loss;
    }
};

#endif //_DEEP_CNN_