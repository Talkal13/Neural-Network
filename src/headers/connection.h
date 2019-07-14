#pragma once
#include <iostream>

template <class T=double>
class connection {
    private:
    std::unordered_map<int, T*> _cons;

    public:
        connection() {
            
        }
        ~connection() {
        }

        void add(T *x) {
            _cons.insert({_cons.size(), x});

        }

        void set(int i, T x) {
            if (_cons.empty() || _cons[i] == nullptr) return;
            *(_cons[i]) = x;
        }

        void sleep() {

        }

        T get(int i) {
            return *(*this)[i];
        }

        T* operator[](int i) {
            if (empty(i)) throw std::out_of_range("Null pointer in connection");
            return (_cons[i]);
        }

        bool empty(int i) {
            return (_cons.empty() || _cons[i] == nullptr);
        }

        void clear() {
            for (std::pair<const int, double *> e : _cons) {
                delete e.second;
            }
            _cons.clear();
        }

};