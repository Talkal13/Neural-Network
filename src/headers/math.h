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

double f_equal(double x) {
    return x;
}

double der_equal(double x) {
    return 1;
}


//0.55
double reLu(double x) {
    if (x > 0.55) return x;
    else return 0;
}
