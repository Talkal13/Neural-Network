#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <fstream>
#include <random>
#include <signal.h>

#include "headers/nn/neuron.h"
#include "headers/nn/layered_net.h"

const int N = 8;

using namespace std;

volatile bool *t;


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
    l.add_layer(sigmoid, 2, 0.03);
    l.add_layer(sigmoid, 1, 0.03);
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
        sleep(1);
    }

}

void create_layered_net() {
    layered_net l;
    l.add_layer(sigmoid, 2, 0.03);
    l.add_layer(sigmoid, 1, 0.03);
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
    l.add_layer(reLu, 2, 0.03);
    l.add_layer(reLu, 3, 0.03);
    l.add_layer(reLu, 1, 0.03);
    std::vector<double> input;
    input.push_back(0);
    input.push_back(0);
    std::vector<double> error;
    std::vector<double> output;
    error.push_back(0);
    double avg_error = 0;
    l.print();
    while(true) {
        size_t j = 0;
        cout << "Training: " << endl;
        for (size_t i = 0; i < 100; i++) {
            input[0] = rand() % 5; input[1] = rand() % 5;
            l.set_input(input);
            l.forward_run();
            double result = input[0] + input[1];
            error[0] = result;
            double e = l.calculate_error(error);
            avg_error += e;
            if (i % 10 == 0) {
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
            error[0] = actual;
            double e = l.calculate_error(error);
            test_avg += e;
            double result = output[0];
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
        x[i] = neuron(i, sigmoid, exit, &mut, 0.03);
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

std::vector<std::vector<double>> unique() {
    std::vector<std::vector<double>> r;
    r.push_back(std::vector<double>());
    r[0].push_back(4);
    r.push_back(std::vector<double>());
    r[1].push_back(8);
    return r;
}

std::vector<std::vector<double>> power() {
    std::vector<std::vector<double>> r;
    r.push_back(std::vector<double>());
    double k = rand() % 10;
    r[0].push_back(k);
    r.push_back(std::vector<double>());
    r[1].push_back(pow(k, 2));
    return r;
}

std::vector<std::vector<double>> not_f() {
    std::vector<std::vector<double>> r;
    r.push_back(std::vector<double>());
    bool k = rand() % 2;
    r[0].push_back(k);
    r.push_back(std::vector<double>());
    r[1].push_back(!k);
    return r;
}

std::vector<std::vector<double>> and_f() {
    std::vector<std::vector<double>> r;
    r.push_back(std::vector<double>());
    bool k = rand() % 2;
    bool j = rand() % 2;
    r[0].push_back(k);
    r[0].push_back(j);
    r.push_back(std::vector<double>());
    r[1].push_back(k && j);
    return r;
}



void debug() {
    layered_net l;
    
    l.add_layer(tanH, 2, 0.03);
    l.add_layer(tanH, 4, 0.03);
    l.add_layer(tanH, 2, 0.03);
    l.add_layer(tanH, 1, 0.03);
    std::vector<double> input = unique()[0];
    std::vector<double> error = unique()[1];
    l.set_input(input);
    l.print();
    while(false) {
        l.forward_run();
        l.back_propagation(error);
        l.print();
        cout << l.get_output()[0] << endl;
        cout << l.calculate_error(error) << endl;
        sleep(1);
    }
    *t = true;
    l.train(and_f, 2e-6, t, true);
    l.test(and_f, 20);

}


void signal_handler(int signum) {
    *t = false;
}




int main(int argc, char *argv[]) {
    // string filename;
    // if (argc == 2) {
    //     filename = argv[1];
    // } else {
    //     filename = "output/output.dat";
    // }
    t = new bool;
    signal(SIGINT, signal_handler);
    debug();
    delete t;
}

