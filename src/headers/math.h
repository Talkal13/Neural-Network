#pragma once
#include <cmath>

#include "connection.h"



template <class T=double> 
T sum(std::vector<T> w, connection<> a) {
    T result = 0;
    for (size_t i = 0; i < w.size(); i++) {
        result += *a[i] * w[i];
    }
    return result;
}

double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-1.0 * x));
}

double equal(double x) {return x;}

//0.55
double reLu(double x) {
    if (x > 0.55) return x;
    else return 0;
}
