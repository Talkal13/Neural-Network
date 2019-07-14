#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <fstream>

#include "headers/neuron.h"
#include "headers/layered_net.h"

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

void create_layered_net(int size) {
    layered_net l;
    functions f = {f_equal, der_equal};
    l.add_input_layer(f, 1);
    for(int i = 0; i < size; i++) {
        l.add_hidden_layer(f, 1);
    }
    l.add_output_layer(f, 1);
    std::vector<double> input;
    input.push_back(0);
    while(input[0] != -1) {
        cout << "Input: "; cin >> input[0];
        l.set_input(input);
        l.forward_run();
        cout << "Output: " << l.get_output()[0] << endl;

    }
}

void create_dinamic_net(string filename) {
    bool *exit = new bool;
    *exit = false;
    bool *done = new bool;
    *done = false;
    thread t[N];
    neuron x[N];
    std::mutex mut;
    for (int i = 0; i < N; i++) {
        x[i] = neuron(i, f_sigmoid, exit, &mut);
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


int main(int argc, char *argv[]) {
    // string filename;
    // if (argc == 2) {
    //     filename = argv[1];
    // } else {
    //     filename = "output/output.dat";
    // }
    create_layered_net(stoi(argv[1]));

}

