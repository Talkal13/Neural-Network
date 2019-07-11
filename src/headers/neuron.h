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
        int _id;
        int _a;
        bool *_exit;
        std::mutex *_mutex;
        size_t _time;
        connection<> input;
        connection<> output;
        std::vector<double> _w;
        int _z;
        double (*_sigma)(double w);
    

        neuron() {

        }

        neuron(int id, double (*sigma)(double), bool *exit, std::mutex *mutex) {
            _id = id;
            _sigma = sigma;
            _exit = exit;
            _mutex = mutex;
            _time = 0;
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
            while(!(*_exit)) {
                rest();
                calculate(_w, input);
                set_output(_a);
                signal();
            }
            
        }

        std::thread start() {
            return std::thread(neuron::call, this);
        }

        static void call(neuron *x) {
            (*x).run();
        }


        //Operator Overload
        bool operator==(const neuron &node) const {
            return _id == node._id;
        }


    private:

        size_t calculate_time() {
            if (!_time) return _time;
            else return time(NULL) - _time;
        }

        void set_time() {
            _time = time(NULL);
        }

        void set_input(int i, double w) {
            input.set(i, w);
        }

        void set_output(double a) {
            output.set(a, 0);
        }

        double calculate(const std::vector<double> &w, connection<> input) {
            _a = _sigma(sum(w, input));
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


};