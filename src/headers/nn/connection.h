#pragma once
#include <iostream>
#include <map>

template <class T=double>
class connection {
    public:
        connection(T *addr) { _addr = addr;}
        ~connection() {}

        T operator *() {
            return *_addr;
        }

        void operator=(T value) {
            set(value);
        }

        friend std::ostream& operator<<(std::ostream& os, connection<T> dt) {
            os << dt.get();
            return os;
        }

        void operator delete(void *del) {
            delete ((connection<T>*) del)->get();
        }

        void set(T value) {
            *_addr = value;
        }

        T* get() {
            return _addr;
        }

    private:
        T * _addr;

};