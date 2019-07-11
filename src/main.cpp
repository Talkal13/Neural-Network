#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <fstream>

#include "headers/neuron.h"

const int N = 8;

using namespace std;


void write_output(string filename, double *output, bool *exit) {
    ofstream file;
    file.open(filename);
    while(!*exit) {
        file << *output << endl;
        sleep(1);
    }
    file.close();
}


int main(int argc, char *argv[]) {
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = "output/output.dat";
    }


    bool *exit = new bool;
    *exit = false;
    bool *done = new bool;
    *done = false;
    thread t[N];
    neuron x[N];
    std::mutex mut;
    for (int i = 0; i < N; i++) {
        x[i] = neuron(i, f_equal, exit, &mut);
        t[i] = x[i].start();
    }

    double *input = new double;
    double *output = new double;
    x[0].add_input(input,1.0);
    for (int i = 0; i < N - 1; i++) {
        double *dob = new double;
        x[i].add_output(dob);
        x[i+1].add_input(dob, 1.0);
    }
    x[N-1].add_output(output);


    thread out = thread(write_output, filename, output, exit);

    while(*input != -1) {
        cout << "Input: "; cin >> *input;
    }

    *exit = true;

    for(int i = 0; i < N; i++) {
        t[i].join();
    }
    out.join();
    delete input;
    delete exit;

}

