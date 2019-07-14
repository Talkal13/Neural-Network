
#include "neuron.h"

enum L_TYPE {HIDDEN_LAYER, INPUT_LAYER, OUTPUT_LAYER};

class layered_net {
    private:
        std::vector<neuron*> _input;
        std::vector<neuron*> _output;
        std::vector<std::vector<neuron*>> _hidden_layers;
        size_t _count;
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
            std::vector<neuron*> b_layer;
            if (i == 0) b_layer = _input;
            else b_layer = _hidden_layers[i - 1];
            for (neuron *b_n : b_layer) {
                n->add_input(b_n->get_output(), 1);
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

    public:
        layered_net() {
            _count = 0;
            _exit = new bool;
            *_exit = false;
            _mutex = new std::mutex;
        }
        ~layered_net() {
            clear_input(_input);
            clear(_input);
            clear(_hidden_layers);
            clear(_output);
            delete _exit;
            delete _mutex;
        }

        bool set_input(std::vector<double> val) {
            for (size_t i = 0; i < _input.size(); ++i) {
                _input[i]->set_input(0, val[i]);
            }
            return true;
        }

        std::vector<double> get_output() {
            std::vector<double> v;
            for (neuron *n : _output) {
                v.push_back(*(n->get_output()));
            }
            return v;
        }

        void add_hidden_layer(functions fun, size_t size) {
            std::vector<neuron*> layer;
            size_t max_size = _hidden_layers.size();
            for (size_t i = 0; i < size; i++) {
                neuron *n = new neuron(_count, fun.sigma, _exit, _mutex);
                connect(n, max_size);
                layer.push_back(n);
                _count++;
            }
            _hidden_layers.push_back(layer);
        }

        bool add_neuron(functions fun, L_TYPE type=HIDDEN_LAYER, size_t l_id = 0) { 
            neuron *n = new neuron(_count, fun.sigma, _exit, _mutex);
            _count++;
            switch (type) { 
            case HIDDEN_LAYER:
                _hidden_layers[l_id].push_back(n);
                connect(n, l_id);
                break;
            case INPUT_LAYER:
                _input.push_back(n);
                break;
            case OUTPUT_LAYER:
                _output.push_back(n);
                connect(n, _hidden_layers.size() - 1);
                break;
            default:
                delete n;
                _count--;
                return false;
            }
            return true;
        }

        bool add_input_layer(functions fun, size_t size) {
            return override_layer(fun, size, INPUT_LAYER);
        }

        bool add_output_layer(functions fun, size_t size) {
            return override_layer(fun, size, OUTPUT_LAYER);
        }

        void forward_run() {
            forward_run(_input);
            forward_run(_hidden_layers);
            forward_run(_output);
        }


        bool override_layer(functions fun, size_t size, L_TYPE type=HIDDEN_LAYER, size_t l_id=0) {
            if (type == OUTPUT_LAYER) l_id = _hidden_layers.size(); 

            std::vector<neuron*> layer;
            for (size_t i = 0; i < size; i++) {
                neuron *n = new neuron(_count, fun.sigma, _exit, _mutex);
                if (type != INPUT_LAYER) connect(n, l_id);
                else n->add_input(new double, 1);
                layer.push_back(n);
                _count++;
            }

            switch (type) { 
            case HIDDEN_LAYER:
                clear(_hidden_layers[l_id]);
                _hidden_layers[l_id] = layer;
                break;
            case INPUT_LAYER:
                clear(_input);
                _input = layer;
                break;
            case OUTPUT_LAYER:
                clear(_output);
                _output = layer;
                break;
            default:
                for (neuron *n : layer) {
                    delete n;
                    _count--;
                }
                return false;
            }
            return true;
        }

};