#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "math.h"
#include "connection.h"

class neuron {
    public:
        int _id;
        int _a;
        bool *_exit;
        connection<> input;
        connection<> output;
        std::vector<double> _w;
        int _z;
        double (*_sigma)(double w);
    

        neuron() {

        }

        neuron(int id, double (*sigma)(double), bool *exit) {
            _id = id;
            _sigma = sigma;
            _exit = exit;
        }

        ~neuron() {
            delete output[0];
        }

        void add_input(double *con, double w) {
            input.add(con);
            _w.push_back(w);
        }

        void add_output(double *w) {
            output.add(w);
            calculate(_w, input);
            set_output(_a);
            
        }

        double get_output() {
            return *output[0];
        }

        double get_input(int i) {
            return *input[i];
        }

        void run() {
            input.sleep();
            calculate(_w, input);
            set_output(_a);
            
        }

        //Operator Overload
        bool operator==(const neuron &node) const {
            return _id == node._id;
        }


    private:
        void set_input(int i, double w) {
            input.set(i, w);
        }

        void set_output(double a) {
            output.set(a, 0);
        }

        double calculate(const std::vector<double> &w, connection<> input) {
            _a =  _sigma(sum(w, input));
            return _a;
        }

        void gradient_descent() {

        }


};