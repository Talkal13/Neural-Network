#pragma once
#include <cmath>

#include "connection.h"

struct functions {
    double (*sigma)(double);
    double (*der)(double);
    std::string repr;  
};

template <class T=double> 
T sum(std::vector<T> w, std::vector<connection<>> a) {
    T result = 0;
    for (size_t i = 0; i < w.size(); i++) {
        result += *a[i] * w[i];
    }
    return result;
}

double f_sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-1.0 * x));
}

double der_sigmoid(double x) {
    double z = f_sigmoid(x);
    return z * (1 - z);
}

functions sigmoid {f_sigmoid, der_sigmoid, "sigmoid"};

double f_step(double x) {
    return (x > 0.5) ? 0 : 1;
}

double der_step(double x) {
    return 1;
}

functions step {f_step, der_step, "step"};

double f_equal(double x) {
    return x;
}

double der_equal(double x) {
    return 1;
}

functions fun_equal {f_equal, der_equal, "equal"};


//0.55
double f_reLu(double x) {
    if (x > 0) return x;
    else return 0;
}

double der_reLu(double x) {
    return 1;
}

functions reLu {f_reLu, der_reLu, "reLu"};

double f_tanh(double x) {
    return (2 / (1 + std::exp(-2*x))) - 1;
}

double der_tanh(double x) {
    return 1 - pow(f_tanh(x), 2);
}

functions tanH {f_tanh, der_tanh, "tanh"};

double f_sin(double x) {
    return sin(x);
}

double der_sin(double x) {
    return -cos(x);
}

functions sinX {f_sin, der_sin, "sin"};

double random_0_1() {
    return (float) rand()/RAND_MAX;
}
