#ifndef _ADAM_HPP_
#define _ADAM_HPP_


#include "matrix.hpp"
#include <map>

class Adam {
public:
    double lr;
    double beta1;
    double beta2;
    int iter = 0;
    std::map<std::string, Matrix<double> *> *m = nullptr;
    std::map<std::string, Matrix<double> *> *v = nullptr;

    explicit Adam(double lr = 0.001, double beta1 = 0.9, double beta2 = 0.999) : lr(lr), beta1(beta1), beta2(beta2) {}


    void update(std::map<std::string, Matrix<double> *> *params, std::map<std::string, Matrix<double> *> *grads) {
        if (this->m == nullptr) {
            this->m = new std::map<std::string, Matrix<double> *>;
            this->v = new std::map<std::string, Matrix<double> *>;
            for (auto &i: *params) {
                (*this->m)[i.first] = new Matrix<double>(i.second->height, i.second->width);
                (*this->v)[i.first] = new Matrix<double>(i.second->height, i.second->width);
            }
        }

        this->iter += 1;
        double lr_t = this->lr * sqrt(1.0 - pow(this->beta2, this->iter)) / (1.0 - pow(this->beta1, this->iter));

        std::vector<std::string> keys;
        for (auto &p: *params) {
            keys.push_back(p.first);
        }
        for (auto &key: keys) {
            //self.m[key] += (1 - self.beta1) * (grads[key] - self.m[key])
            auto sub_1 = (*grads)[key]->operator-((*this->m)[key]);
            auto re1 = sub_1->operator*(1 - this->beta1);
            auto out1 = (*this->m)[key]->operator+(re1);
            delete (sub_1);
            delete (re1);
            delete ((*this->m)[key]);
            (*this->m)[key] = out1;

            //self.v[key] += (1 - self.beta2) * (grads[key]**2 - self.v[key])
            auto pow2 = (*grads)[key]->mat_pow(2);
            auto sub_2 = pow2->operator-((*this->v)[key]);
            auto re2 = sub_2->operator*(1 - this->beta2);
            auto out2 = (*this->v)[key]->operator+(re2);
            delete (pow2);
            delete (sub_2);
            delete (re2);
            delete ((*this->v)[key]);
            (*this->v)[key] = out2;


            //params[key] -= lr_t * self.m[key] / (np.sqrt(self.v[key]) + 1e-7)
            auto sqrt_ = (*this->v)[key]->mat_pow(0.5);
            auto sqrt_add = sqrt_->operator+(1e-7);
            auto lr_t_mult_m = (*this->m)[key]->operator*(-lr_t);
            auto re = lr_t_mult_m->operator/(sqrt_add);
            auto out_final = (*params)[key]->operator+(re);
            delete (sqrt_);
            delete (sqrt_add);
            delete (lr_t_mult_m);
            delete (re);
            delete ((*params)[key]);
            (*params)[key] = out_final;
        }
    }
};

#endif //_ADAM_HPP_