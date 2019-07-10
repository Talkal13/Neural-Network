#pragma once
#include <iostream>

template <class T>
struct signal_pr {
    T *ptr;
    T old;
};

template <class T=double>
class connection {
    private:
    std::unordered_map<int, signal_pr<T>> _cons;

    public:
        connection() {
            
        }
        ~connection() {
        }

        void add(T *x) {
            signal_pr<T> p;
            (p).ptr = x;
            (p).old = *x;
            _cons.insert({_cons.size(), p});

        }

        void set(T x, int i) {
            *((_cons[i]).ptr) = x;
        }

        void sleep() {
            bool change;
            do {
                change = false;
                for (auto it = _cons.begin(); it != _cons.end(); ++it) {
                    signal_pr<T> p = (*it).second;
                    if (*p.ptr != p.old) {
                        change = true; 
                        p.old = *p.ptr;
                    }
                }
            } while(!change);
        }

        T get(int i) {
            return *(_cons[i].ptr);
        }

        T* operator[](int i) {
            return (_cons[i]).ptr;
        }

};