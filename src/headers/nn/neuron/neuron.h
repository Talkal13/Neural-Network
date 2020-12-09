#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "math.h"
#include "connection.h"

class neuron {
    public:
        
        neuron() {}

        neuron(int id, functions fun, bool *exit, std::mutex *mutex, double lr) {
            _id = id;
            _sigma = fun.sigma;
            _der_sigma = fun.der;
            _repr_fun = fun.repr;
            _exit = exit;
            _mutex = mutex;
            _lr = lr;
            _output.push_back(connection<>());

            _a = 0;
            _z = 0;
        }

        ~neuron() {
            try {
                delete _output[0].get();
                delete _delta_o[0].get();
            } catch (std::out_of_range &e) {};
        }

        void add_input(double *con, double w) {
            _input.push_back(connection<>(con));
            _w.push_back(w);
            _delta_o.push_back(connection<>(new double));
        }

        void add_output(double *w) {
            _output.push_back(connection<>(w));
            calculate(_w, _input);
            set_output(_a);
        }

        void add_i_error(double *error) {
            _delta_i.push_back(connection<>(error));
        }

        double* get_o_error(int i) {
            return _delta_o[i].get();
        }

        double* get_output() {
            return _output[0].get();
        }

        double get_input(int i) {
            return *_input[i];
        }

        double get_lr() {
            return _lr;
        }

        std::string get_repr() {
            return _repr_fun;
        }

        void run() {
            while(!(*_exit)) {
                rest();
                update_gates();
                forward_run();
                signal();
            }
            
        }

        void forward_run() {
            calculate(_w, _input);
            set_output(_a);
        }

        void back_propagation() {
            _delta = calculate_delta(); // error
            update_delta();
            for (size_t i = 0; i < _w.size(); i++) {
                _w[i] = _w[i] - calculate_gradient(i) * _lr; // TODO: learning rate;
            }
            _b = _b - _delta * _lr;
            
        }

        std::thread start() {
            return std::thread(neuron::call, this);
        }


        //Operator Overload
        bool operator==(const neuron &node) const {
            return _id == node._id;
        }

        void set_input(int i, double w) {
            _input[i] = w;
        }

        void set_delta_error(double y_j) {
            _delta_i[0] = (_a - y_j);
        }

        void clear_input() {
            _input.clear();
        }

        void clear_error() {
            _delta_i.clear();
        }

        void print() {
            std::cout << _id << ": "
                << "\t" << "Input: " << std::endl;
                for (size_t i = 0; i < _input.size(); i++) {
                    std::cout << "\t\t" << _input[i] << "  w" << i << ": " << _w[i] << std::endl;
                }
                std::cout << "\t" << "Output: " << std::endl;
                for (auto out : _output) {
                    std::cout << "\t\t" << out << std::endl;
                }
                std::cout << "\t" << "Input Error: " << std::endl;
                for (auto in : _delta_i) {
                    std::cout << "\t\t" << in << std::endl;
                }
                std::cout << "\t" << "Output Error:" << std::endl;
                for (auto out : _delta_o) {
                    std::cout << "\t\t" << out <<std::endl;
                }
            std::cout << std::endl;

        }

    private:

        static void call(neuron *x) {
            x->run();
        }

        void set_output(double a) {
            _output[0] = a;
        }

        double calculate(const std::vector<double> &w, std::vector<connection<>> _input) {
            _z = sum<double>(w, _input) + _b;
            _a = _sigma(_z);
            return _a;
        }

        /*
            Resting function -> Check for changes if found gain the mutex and trigger a wake up        
         */
        void rest() {
            _mutex->lock();
        }

        void signal() {
            _mutex->unlock();
        }

        void update_gates() {
            for (size_t i = 0; i < _w.size(); i++) {
                try {
                    _w[i] += *_delta_i[i];
                } catch (std::out_of_range& e) {};
            }
        }

        double calculate_gradient(int j) {
            return _delta * *_input[j];
        }

        double calculate_delta() {
            double delta = 0;
            for (auto d : _delta_i) {
                delta += *d;
            }
            return delta * _der_sigma(_z);
        }

        void update_delta() {
            for (size_t i = 0; i < _delta_o.size(); i++) {
                update_delta(i);
            }
        }

        void update_delta(int i) {
            _delta_o[i] = _w[i] * _delta;
        }

        int _id;
        double _a;
        double _z;
        double _lr;
        bool *_exit;
        double _delta;
        std::mutex *_mutex;
        std::vector<connection<>> _input;
        std::vector<connection<>> _output;
        std::vector<connection<>> _delta_i;
        std::vector<connection<>> _delta_o;
        std::vector<double> _w;
        double _b;
        double (*_sigma)(double w);
        double (*_der_sigma)(double w);
        std::string _repr_fun;


};