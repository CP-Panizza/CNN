#ifndef _DEEP_CNN_
#define _DEEP_CNN_

#include "layers.h"
#include "libs/rapidjson/document.h"
#include "libs/rapidjson/writer.h"
#include "libs/rapidjson/stringbuffer.h"


class DeepCnn {
public:
    Conv *conv1;
    ConvRelu *relu2;
    Conv *conv3;
    ConvRelu *relu4;
    Pooling *pooling5;
    ConvDropout *dropout6;

    Conv *conv7;
    ConvRelu *relu8;
    Conv *conv9;
    ConvRelu *relu10;
    Pooling *pooling11;
    ConvDropout *dropout12;

    PoolingAffine *poolingAffine13;
    Relu *relu14;
    Dropout *dropout15;
    Affine *affine16;
    Softmax *softmax17;

    Adam *adam;
    double lr = 0.01;

    DeepCnn(std::vector<int> &input_shape) {
        int filter_size = 5;
        int stride = 1;
        int pad = 2;
        int hidden_size = 50;
        int output = 2;

        /**
         * He方式初始化权重参数
         */
        Matrix<double> pre_node_nums = {1 * 5 * 5,
                                        16 * 5 * 5,
                                        16 * 5 * 5,
                                        32 * 5 * 5,
                                        32 * 5 * 5,
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
        dropout6 = new ConvDropout(0.25);

        conv7 = new Conv(wight_init_scales->Get(0, 2), 32, filter_size, stride, pad, pooling5->out_shape);
        relu8 = new ConvRelu;
        conv9 = new Conv(wight_init_scales->Get(0, 3), 32, filter_size, stride, pad, conv7->out_shape);
        relu10 = new ConvRelu;
        pooling11 = new Pooling(2, 2, conv9->out_shape);
        dropout12 = new ConvDropout(0.25);

        poolingAffine13 = new PoolingAffine(wight_init_scales->Get(0, 4), pooling11->out_shape, hidden_size);
        relu14 = new Relu;
        dropout15 = new Dropout(0.25);
        affine16 = new Affine(wight_init_scales->Get(0, 5), poolingAffine13->out_shape, output);
        softmax17 = new Softmax;
        adam = new Adam(this->lr);
    }

    double train(std::vector<std::vector<Matrix<double> *> > *x, Matrix<double> *la) {
        //forward
        auto conv1_out = conv1->forward(x);
        auto relu2_out = relu2->forward(conv1_out);
        auto conv3_out = conv3->forward(relu2_out);
        auto relu4_out = relu4->forward(conv3_out);
        auto pooling5_out = pooling5->forward(relu4_out);
        auto dropout6_out = dropout6->forward(pooling5_out);

        auto conv7_out = conv7->forward(dropout6_out);
        auto relu8_out = relu8->forward(conv7_out);
        auto conv9_out = conv9->forward(relu8_out);
        auto relu10_out = relu10->forward(conv9_out);
        auto pooling11_out = pooling11->forward(relu10_out);
        auto dropout12_out = dropout12->forward(pooling11_out);


        auto poolingAffine13_out = poolingAffine13->forward(dropout12_out);
        auto relu14_out = relu14->forward(poolingAffine13_out);
        auto dropout15_out = dropout15->forword(relu14_out);
        auto affine16_out = affine16->forward(dropout15_out);
        auto loss = softmax17->forward(affine16_out, la);

        free_data(conv1_out);
        free_data(relu2_out);
        free_data(conv3_out);
        free_data(relu4_out);
        free_data(pooling5_out);
        free_data(dropout6_out);
        free_data(conv7_out);
        free_data(relu8_out);
        free_data(conv9_out);
        free_data(relu10_out);
        free_data(pooling11_out);
        free_data(dropout12_out);
        delete (poolingAffine13_out);
        delete (relu14_out);
        delete (dropout15_out);
        delete (affine16_out);




        //backward
        auto softmax17_dout = softmax17->backward();
        auto affine16_dout = affine16->backward(softmax17_dout);
        auto dropout15_dout = dropout15->backward(affine16_dout);
        auto relu14_dout = relu14->backward(dropout15_dout);
        auto poolingAffine13_dout = poolingAffine13->backword(relu14_dout);
        auto dropout12_dout = dropout12->backward(poolingAffine13_dout);
        auto pooling11_dout = pooling11->backword(dropout12_dout);
        auto relu10_dout = relu10->backword(pooling11_dout);
        auto conv9_dout = conv9->backword(relu10_dout);
        auto relu8_dout = relu8->backword(conv9_dout);
        auto conv7_dout = conv7->backword(relu8_dout);
        auto dropout6_dout = dropout6->backward(conv7_dout);
        auto pooling5_dout = pooling5->backword(dropout6_dout);
        auto relu4_dout = relu4->backword(pooling5_dout);
        auto conv3_dout = conv3->backword(relu4_dout);
        auto relu2_dout = relu2->backword(conv3_dout);
        auto conv1_dout = conv1->backword(relu2_dout);


        free_data(conv1_dout);
        free_data(relu2_dout);
        free_data(conv3_dout);
        free_data(relu4_dout);
        free_data(pooling5_dout);
        free_data(dropout6_dout);
        free_data(conv7_dout);
        free_data(relu8_dout);
        free_data(conv9_dout);
        free_data(relu10_dout);
        free_data(pooling11_dout);
        free_data(dropout12_dout);
        delete (poolingAffine13_dout);
        delete (relu14_dout);
        delete (dropout15_dout);
        delete (affine16_dout);
        delete(softmax17_dout);

        auto params = new std::map<std::string, Matrix<double> *>;
        params->insert(std::pair<std::string, Matrix<double> *>("conv1_w", conv1->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv1_b", conv1->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv3_w", conv3->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv3_b", conv3->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv7_w", conv7->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv7_b", conv7->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("conv9_w", conv9->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("conv9_b", conv9->b->Copy()));


        params->insert(std::pair<std::string, Matrix<double> *>("fc13_w", poolingAffine13->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("fc13_b", poolingAffine13->b->Copy()));

        params->insert(std::pair<std::string, Matrix<double> *>("fc16_w", affine16->W->Copy()));
        params->insert(std::pair<std::string, Matrix<double> *>("fc16_b", affine16->b->Copy()));

        auto grads = new std::map<std::string, Matrix<double> *>;
        grads->insert(std::pair<std::string, Matrix<double> *>("conv1_w", conv1->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv1_b", conv1->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv3_w", conv3->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv3_b", conv3->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv7_w", conv7->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv7_b", conv7->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("conv9_w", conv9->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("conv9_b", conv9->db));



        grads->insert(std::pair<std::string, Matrix<double> *>("fc13_w", poolingAffine13->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("fc13_b", poolingAffine13->db));

        grads->insert(std::pair<std::string, Matrix<double> *>("fc16_w", affine16->dW));
        grads->insert(std::pair<std::string, Matrix<double> *>("fc16_b", affine16->db));

        this->adam->update(params, grads);


        delete (this->conv1->W);
        this->conv1->W = (*params)["conv1_w"];
        delete (this->conv1->b);
        this->conv1->b = (*params)["conv1_b"];


        delete (this->conv3->W);
        this->conv3->W = (*params)["conv3_w"];
        delete (this->conv3->b);
        this->conv3->b = (*params)["conv3_b"];


        delete (this->conv7->W);
        this->conv7->W = (*params)["conv7_w"];
        delete (this->conv7->b);
        this->conv7->b = (*params)["conv7_b"];


        delete (this->conv9->W);
        this->conv9->W = (*params)["conv9_w"];
        delete (this->conv9->b);
        this->conv9->b = (*params)["conv9_b"];



        delete (this->poolingAffine13->W);
        this->poolingAffine13->W = (*params)["fc13_w"];
        delete (this->poolingAffine13->b);
        this->poolingAffine13->b = (*params)["fc13_b"];


        delete (this->affine16->W);
        this->affine16->W = (*params)["fc16_w"];
        delete (this->affine16->b);
        this->affine16->b = (*params)["fc16_b"];

        delete (params);
        delete (grads);

        return loss;
    }


    Matrix<double> *predict(std::vector<std::vector<Matrix<double> *> > *x, Matrix<double> *la) {
        //forward
        auto conv1_out = conv1->forward(x);
        auto relu2_out = relu2->forward(conv1_out);
        auto conv3_out = conv3->forward(relu2_out);
        auto relu4_out = relu4->forward(conv3_out);
        auto pooling5_out = pooling5->forward(relu4_out);
        auto dropout6_out = dropout6->forward(pooling5_out);

        auto conv7_out = conv7->forward(dropout6_out);
        auto relu8_out = relu8->forward(conv7_out);
        auto conv9_out = conv9->forward(relu8_out);
        auto relu10_out = relu10->forward(conv9_out);
        auto pooling11_out = pooling11->forward(relu10_out);
        auto dropout12_out = dropout12->forward(pooling11_out);


        auto poolingAffine13_out = poolingAffine13->forward(dropout12_out);
        auto relu14_out = relu14->forward(poolingAffine13_out);
        auto dropout15_out = dropout15->forword(relu14_out);
        auto affine16_out = affine16->forward(dropout15_out);
        auto loss = softmax17->forward(affine16_out, la);

        free_data(conv1_out);
        free_data(relu2_out);
        free_data(conv3_out);
        free_data(relu4_out);
        free_data(pooling5_out);
        free_data(dropout6_out);
        free_data(conv7_out);
        free_data(relu8_out);
        free_data(conv9_out);
        free_data(relu10_out);
        free_data(pooling11_out);
        free_data(dropout12_out);
        delete (poolingAffine13_out);
        delete (relu14_out);
        delete (dropout15_out);
        delete (affine16_out);

        return softmax17->y->Copy();
    }

//    void save_param(const std::string &path) {
//        std::cout << "save_param...\n";
//        rapidjson::StringBuffer s;
//        rapidjson::Writer<rapidjson::StringBuffer> w(s);
//        w.StartObject();
//        w.Key("conv1_w");
//        w.StartArray();
//        for (int i = 0; i < this->conv1->W->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv1->W->width; ++j) {
//                w.Double(this->conv1->W->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//        //
//        w.Key("conv1_b");
//        w.StartArray();
//        for (int i = 0; i < this->conv1->b->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv1->b->width; ++j) {
//                w.Double(this->conv1->b->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//
//        w.Key("conv3_w");
//        w.StartArray();
//        for (int i = 0; i < this->conv3->W->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv3->W->width; ++j) {
//                w.Double(this->conv3->W->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//        //
//        w.Key("conv3_b");
//        w.StartArray();
//        for (int i = 0; i < this->conv3->b->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv3->b->width; ++j) {
//                w.Double(this->conv3->b->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//
//
//        //
//        w.Key("conv6_w");
//        w.StartArray();
//        for (int i = 0; i < this->conv6->W->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv6->W->width; ++j) {
//                w.Double(this->conv6->W->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//        //
//        w.Key("conv6_b");
//        w.StartArray();
//        for (int i = 0; i < this->conv6->b->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv6->b->width; ++j) {
//                w.Double(this->conv6->b->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//        //
//        w.Key("conv8_w");
//        w.StartArray();
//        for (int i = 0; i < this->conv8->W->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv8->W->width; ++j) {
//                w.Double(this->conv8->W->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//        //
//        w.Key("conv8_b");
//        w.StartArray();
//        for (int i = 0; i < this->conv8->b->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->conv8->b->width; ++j) {
//                w.Double(this->conv8->b->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//        w.EndObject();
//
//
//        w.Key("fc11_w");
//        w.StartArray();
//        for (int i = 0; i < this->poolingAffine11->W->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->poolingAffine11->W->width; ++j) {
//                w.Double(this->poolingAffine11->W->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//
//        //
//        w.Key("fc11_b");
//        w.StartArray();
//        for (int i = 0; i < this->poolingAffine11->b->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->poolingAffine11->b->width; ++j) {
//                w.Double(this->poolingAffine11->b->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//
//        w.Key("fc14_w");
//        w.StartArray();
//        for (int i = 0; i < this->affine14->W->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->affine14->W->width; ++j) {
//                w.Double(this->affine14->W->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//
//        //
//        w.Key("fc14_b");
//        w.StartArray();
//        for (int i = 0; i < this->affine14->b->height; ++i) {
//            w.StartArray();
//            for (int j = 0; j < this->affine14->b->width; ++j) {
//                w.Double(this->affine14->b->Get(i, j));
//            }
//            w.EndArray();
//        }
//        w.EndArray();
//
//
//        string data(s.GetString());
//
//        std::ofstream f(path);
//        if (f.is_open()) {
//            f << data;
//        } else {
//            perror("open outfile err!");
//        }
//        f.close();
//    }


//    void load_param(const std::string &path) {
//        std::string param = read_file(path);
//        rapidjson::Document d;
//        if (d.Parse(param.c_str()).HasParseError()) {
//            printf("load_param error!\n");
//            exit(-1);
//        }
//        rapidjson::Document::Array conv1_w_ = d["conv1_w"].GetArray();
//        for (int i = 0; i < conv1_w_.Size(); ++i) {
//            rapidjson::Document::Array row = conv1_w_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv1->W->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array conv1_b_ = d["conv1_b"].GetArray();
//        for (int i = 0; i < conv1_b_.Size(); ++i) {
//            rapidjson::Document::Array row = conv1_b_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv1->b->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array conv3_w_ = d["conv3_w"].GetArray();
//        for (int i = 0; i < conv3_w_.Size(); ++i) {
//            rapidjson::Document::Array row = conv3_w_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv3->W->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array conv3_b_ = d["conv3_b"].GetArray();
//        for (int i = 0; i < conv3_b_.Size(); ++i) {
//            rapidjson::Document::Array row = conv3_b_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv3->b->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array conv6_w_ = d["conv6_w"].GetArray();
//        for (int i = 0; i < conv6_w_.Size(); ++i) {
//            rapidjson::Document::Array row = conv6_w_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv6->W->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array conv6_b_ = d["conv6_b"].GetArray();
//        for (int i = 0; i < conv6_b_.Size(); ++i) {
//            rapidjson::Document::Array row = conv6_b_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv6->b->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array conv8_w_ = d["conv8_w"].GetArray();
//        for (int i = 0; i < conv8_w_.Size(); ++i) {
//            rapidjson::Document::Array row = conv8_w_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv8->W->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array conv8_b_ = d["conv8_b"].GetArray();
//        for (int i = 0; i < conv6_b_.Size(); ++i) {
//            rapidjson::Document::Array row = conv6_b_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->conv8->b->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//
//
//
//        rapidjson::Document::Array fc11_w_ = d["fc11_w"].GetArray();
//        for (int i = 0; i < fc11_w_.Size(); ++i) {
//            rapidjson::Document::Array row = fc11_w_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->poolingAffine11->W->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array fc11_b_ = d["fc11_b"].GetArray();
//        for (int i = 0; i < fc11_b_.Size(); ++i) {
//            rapidjson::Document::Array row = fc11_b_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->poolingAffine11->b->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array fc14_w_ = d["fc14_w"].GetArray();
//        for (int i = 0; i < fc14_w_.Size(); ++i) {
//            rapidjson::Document::Array row = fc14_w_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->affine14->W->Set(i, j, row[j].GetDouble());
//            }
//        }
//
//        rapidjson::Document::Array fc14_b_ = d["fc14_b"].GetArray();
//        for (int i = 0; i < fc14_b_.Size(); ++i) {
//            rapidjson::Document::Array row = fc14_b_[i].GetArray();
//            for (int j = 0; j < row.Size(); ++j) {
//                this->affine14->b->Set(i, j, row[j].GetDouble());
//            }
//        }
//    }
};

#endif //_DEEP_CNN_