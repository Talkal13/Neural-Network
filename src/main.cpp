#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <mutex>

#include "neuron.h"

const int N = 8;

using namespace std;


int main(int argc, char *argv[]) {

    bool *exit = new bool;
    *exit = false;
    bool *done = new bool;
    *done = false;
    thread t[N];
    neuron x[N];
    std::mutex mut;
    for (int i = 0; i < N; i++) {
        x[i] = neuron(i, sigmoid, exit, &mut);
        t[i] = x[i].start();
    }

    double *input = new double;
    *input = 13;
    double *output = new double;
    x[0].add_input(input,1.0);
    for (int i = 0; i < N - 1; i++) {
        double *dob = new double;
        x[i].add_output(dob);
        x[i+1].add_input(dob, 1.0);
    }
    x[N-1].add_output(output);
    x[N-1].add_done(done);
     
    while(*input != -1) {
        cout << "Input: "; cin >> *input;
        while(!(*done));
        cout << "Output: " << *output << endl;
    }

    *exit = true;

    for(int i = 0; i < N; i++) {
        t[i].join();
    }
    delete input;
    delete exit;

}

