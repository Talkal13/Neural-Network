#pragma once
#include <cmath>

#include "connection.h"

struct functions {
    double (*sigma)(double);
    double (*der)(double);    
};

template <class T=double> 
T sum(std::vector<T> w, connection<> a) {
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

functions sigmoid {f_sigmoid, der_sigmoid};

double f_step(double x) {
    return (x > 0.5) ? 0 : 1;
}

double der_step(double x) {
    return 1;
}

functions step {f_step, der_step};

double f_equal(double x) {
    return x;
}

double der_equal(double x) {
    return 1;
}

functions fun_equal {f_equal, der_equal};


//0.55
double f_reLu(double x) {
    if (x > 0) return x;
    else return 0;
}

double der_reLu(double x) {
    return 1;
}

functions reLu {f_reLu, der_reLu};

double random_0_1() {
    return (rand() / (RAND_MAX)) + 1;
}
