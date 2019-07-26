#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <fstream>
#include <random>

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

void f_and() {
    layered_net l;
    l.add_layer(sigmoid, 2);
    l.add_layer(sigmoid, 3);
    l.add_layer(sigmoid, 3);
    l.add_layer(sigmoid, 2);
    std::vector<double> input;
    std::vector<double> error;
    input.push_back(0);
    input.push_back(0);
    error.push_back(0);
    error.push_back(0);
    while(true) {
        input[0] = rand() % 2; input[1] = rand() % 2;
        l.set_input(input);
        l.forward_run();
        std::vector<double> output = l.get_output();
        //bool out = (output[0] < output[1]);
        bool actual = input[0] && input[1];
        error[0] = (!actual) ? 1 : 0;
        error[1] = (actual) ? 1 : 0;
        cout << l.calculate_error(error) << endl;
        l.back_propagation(error);
        l.print();
        sleep(1);
    }

}

void create_layered_net() {
    layered_net l;
    l.add_layer(sigmoid, 2);
    l.add_layer(sigmoid, 1);
    std::vector<double> input;
    std::vector<double> error;
    error.push_back(0);
    input.push_back(0);
    input.push_back(0);
    while(true) {
        input[0] = rand() % 5; input[1] = rand() % 5;
        l.set_input(input);
        l.forward_run();
        cout << input[0] << "+" << input[1] << " = " << l.get_output()[0] << endl;
        error[0] = input[0] + input[1];
        l.back_propagation(error);
        l.print();
        sleep(1);

    }
}

void sum() {
    layered_net l;
    l.add_layer(reLu, 2);
    l.add_layer(sigmoid, 2);
    l.add_layer(sigmoid, 10);
    std::vector<double> input;
    input.push_back(0);
    input.push_back(0);
    std::vector<double> error;
    std::vector<double> output;
    for (int i = 0; i < 10; i++) {
        error.push_back(0);
    }
    double avg_error = 0;
    l.print();
    while(true) {
        size_t j = 0;
        cout << "Training: " << endl;
        for (size_t i = 0; i < 100000; i++) {
            input[0] = rand() % 5; input[1] = rand() % 5;
            l.set_input(input);
            l.forward_run();
            int result = input[0] + input[1];
            for (int k = 0; k < 10; k++) {
                error[k] = (k == result) ? 1 : 0;
            }
            double e = l.calculate_error(error);
            avg_error += e;
            if (i % 1000 == 0) {
                j++;
                cout << "[";
                for (size_t k = 0; k < j; k++) {
                    cout << "#";
                }
                for (size_t k = j; k < 100; k++) {
                    cout << " ";
                }
                cout << "]  Error = " << e;
                cout << "\r";
                std::cout.flush();
            }
            
            l.back_propagation(error);
        }
        avg_error /= 100000;
        cout << endl << "Average training error: " << avg_error <<  endl << endl << "Test:" << endl;
        
        double test_avg = 0;
        for (int i = 0; i < 10; i++) {
            output = l.get_output();
            input[0] = rand() % 5; input[1] = rand() % 5;
            l.set_input(input);
            l.forward_run();
            double actual = input[0] + input[1];
            for (int k = 0; k < 10; k++) {
                error[k] = (k == actual) ? 1 : 0;
            }
            double e = l.calculate_error(error);
            test_avg += e;
            int result = 0;
            double foo = 0;
            for (int k = 0; k < 10; k++) {
                if (output[k] > foo)  {result = k; foo = output[k];}
            }
            cout << input[0] << " + " << input[1] << " = " << result << " (" << ((actual == result) ? "+" : "-") << ")" << endl;
            l.back_propagation(error);

        }
        avg_error = 0;
        cout << "Average error: " << test_avg / 10 << endl << endl;
        
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
        x[i] = neuron(i, sigmoid, exit, &mut);
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
   sum();
}

