#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include "neuron.h"

const int N = 8;

using namespace std;

void run(neuron *x) {
    while(!*(*x)._exit) {
        (*x).run();    
    }
}

void run_print(neuron *x) {
    while(!*(*x)._exit) {
        (*x).run();
        
    }
}


int main(int argc, char *argv[]) {
    bool *exit = new bool;
    *exit = false;
    neuron x[N];
    for (int i = 0; i < N; i++)
        x[i] = neuron(i, equal, exit);

    thread t[N];

    double *input = new double;
    *input = 13;
    double *output = new double;
    x[0].add_input(input,1.0);
    for (int i = 0; i < N - 1; i++) {
        double *dob = new double;
        x[i].add_output(dob);
        x[i+1].add_input(dob, 1.0);
        t[i] = thread(run, &x[i]);
    }
    x[N-1].add_output(output);
    t[N-1] = thread(run, &x[N-1]);
    //double in = 0;
    while(*input != -1) {
        cout << "Input: ";
        cin >> *input;
        while(*input != x[N-1].get_output());
        cout << "Output: " << *output << endl;
    }

    *exit = true;

    for(int i = 0; i < N; i++) {
        t[i].join();
    }

    delete input;
    delete exit;

}