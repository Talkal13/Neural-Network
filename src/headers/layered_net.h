
#include "neuron.h"

enum L_TYPE {HIDDEN_LAYER, INPUT_LAYER, OUTPUT_LAYER};

class layered_net {
    private:
        std::vector<std::vector<neuron*>> _layers;
        size_t _count;
        size_t _l_size;
        bool *_exit;
        std::mutex *_mutex;

        template <class T=neuron*>
        void clear(std::vector<T> layer) {
            for (T e : layer) {
                clear(e);
            }
        }

        void clear(neuron *n) {
            delete n;
        }

        template <class T=neuron*>
        void clear_input(std::vector<T> layer) {
            for (T e : layer) {
                clear_input(e);
            }
        }

        void clear_input(neuron *n) {
            n->clear_input();
        }

        void connect(neuron *n, size_t i) {
            if (i < 0) return;
            std::vector<neuron*> b_layer = _layers[i];
            for (neuron *b_n : b_layer) {
                n->add_input(b_n->get_output(), random_0_1());
            }
        }

        void connect_error(neuron *n, size_t i, size_t j) {
            std::vector<neuron*> b_layer = _layers[i];
            for (neuron *b_n : b_layer) {
                n->add_i_error(b_n->get_o_error(j));
            }

        }

        template <class T=neuron*>
        void forward_run(std::vector<T> layer) {
            for (T element : layer) {
                forward_run(element);
            }
        }

        void forward_run(neuron *n) {
            n->forward_run();
        }

        template <class T=neuron*>
        void back_propagation(std::vector<T> layer) {
            for (auto it = layer.rbegin(); it != layer.rend(); ++it) {
                back_propagation(*it);
            }
        }

        void back_propagation(neuron *n) {
            n->back_propagation();
        }

    public:
        layered_net() {
            _count = 0;
            _exit = new bool;
            *_exit = false;
            _mutex = new std::mutex;
            _l_size = 0;
        }
        ~layered_net() {
            clear_input(_layers[0]);
            clear(_layers);
            delete _exit;
            delete _mutex;
        }

        bool set_input(std::vector<double> val) {
            for (size_t i = 0; i < _layers[0].size(); ++i) {
                _layers[0][i]->set_input(0, val[i]);
            }
            return true;
        }

        std::vector<double> get_output() {
            std::vector<double> v;
            for (neuron *n : _layers[_l_size - 1]) {
                v.push_back(*(n->get_output()));
            }
            return v;
        }


        bool add_neuron(functions fun, size_t l_id = 0) {
            if (l_id <= _layers.size()) return false;
            neuron *n = new neuron(_count, fun, _exit, _mutex);
            _count++;
            _layers[l_id].push_back(n);
            if (l_id != 0)
                connect(n, l_id - 1);

            if (l_id != _l_size)
                connect_error(n, l_id + 1, _layers[l_id].size() - 1);

            return true;
        }

        bool add_input_layer(functions fun, size_t size) {
            return override_layer(fun, size, 0);
        }

        bool add_layer(functions fun, size_t size) {
            std::vector<neuron*> layer;
            for (size_t i = 0; i < size; i++) {
                neuron *n = new neuron(_count, fun, _exit, _mutex);
                layer.push_back(n);
                _count++;
            }

            _layers.push_back(layer);
            
            if (_l_size == 0) {
                for (neuron *j : layer) {
                    j->add_input(new double, 1);
                    j->add_i_error(new double);
                }
            } else {
                for (neuron *j : layer) {
                    connect(j, _l_size - 1);
                    j->add_i_error(new double);
                }
                for (size_t j = 0; j < _layers[_l_size - 1].size(); j++) {
                    neuron *k = _layers[_l_size - 1][j];
                    k->clear_error();
                    connect_error(k, _l_size, j);
                }
            }
            _l_size++;
            return true;
        }

        bool add_output_layer(functions fun, size_t size) {
            return override_layer(fun, size, _l_size);
        }

        void forward_run() {
            forward_run(_layers);
        }

        void back_propagation(std::vector<double> error) {
            for (size_t i = 0; i < _layers[_l_size - 1].size(); i++) {
                _layers[_l_size - 1][i]->set_delta_error(error[i]);
            }
            back_propagation(_layers);
        }

        double calculate_error(std::vector<double> y) {
            double error = 0;
            std::vector<double> a = get_output();
            for (size_t i = 0; i < y.size(); i++) {
                error += std::pow(y[i] - a[i], 2);
            }
            return error / 2;
        }


        bool override_layer(functions fun, size_t size, size_t l_id=0) { 
            std::vector<neuron*> layer;
            for (size_t i = 0; i < size; i++) {
                neuron *n = new neuron(_count, fun, _exit, _mutex);
                layer.push_back(n);
                _count++;
            }


            auto it = _layers.begin() + l_id;

            if (_layers.size() == 0) {
                _layers.push_back(layer);
            } else {
                _layers.insert(it, layer);
            }
            size_t k = 0;
            for (neuron *j : *it) {
                if (_layers.size() == 1) return true;
                if (l_id == 0) {
                    j->add_input(new double, random_0_1());
                    connect_error(j, l_id + 1, k);
                }
                else if (l_id == _l_size) {
                    connect(j, l_id - 1);
                    j->add_i_error(new double);
                }
                else {
                    connect(j, l_id - 1);
                    connect_error(j, l_id + 1, k);
                }
                k++;
            }


        }

        void print() {
            std::cout << "Layered Net: " << std::endl;
            for (auto layer : _layers) {
                for (neuron* n : layer ) {
                    n->print();
                }
            }
        }

};