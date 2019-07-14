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
        neuron() {

        }

        neuron(int id, double (*sigma)(double), bool *exit, std::mutex *mutex) {
            _id = id;
            _sigma = sigma;
            _exit = exit;
            _mutex = mutex;
            double *d = new double;
            _output.add(d);
        }

        ~neuron() {
            try {
                delete _output[0];
            } catch (std::out_of_range &e) {};
        }

        void add_input(double *con, double w) {
            _input.add(con);
            _w.push_back(w);
        }

        void add_output(double *w) {
            _output.add(w);
            calculate(_w, _input);
            set_output(_a);
            
        }

        double* get_output() {
            return _output[0];
        }

        double get_input(int i) {
            return *_input[i];
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

        std::thread start() {
            return std::thread(neuron::call, this);
        }


        //Operator Overload
        bool operator==(const neuron &node) const {
            return _id == node._id;
        }

        void set_input(int i, double w) {
            _input.set(i, w);
        }

        void clear_input() {
            _input.clear();
        }

    private:

        static void call(neuron *x) {
            x->run();
        }

        void set_output(double a) {
            _output.set(0, a);
        }

        double calculate(const std::vector<double> &w, connection<> _input) {
            _a = _sigma(sum(w, _input));
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

        void gradient_descent() {
            
        }

        void update_gates() {
            for (size_t i = 0; i < _w.size(); i++) {
                try {
                    _w[i] += _g_gate.get(i);
                } catch (std::out_of_range& e) {};
            }
        }

        int _id;
        double _a;
        bool *_exit;
        std::mutex *_mutex;
        connection<> _input;
        connection<> _output;
        connection<> _g_gate;
        std::vector<double> _w;
        double (*_sigma)(double w);


};