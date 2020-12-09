#pragma once
#include <iostream>
#include <map>
#include <mutex>

template <class T=double>
class connection {
    public:
        connection() {_addr = new T;}
        ~connection() {}

        // Operators

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

        // Getters and Setters

        void set(T value) {
            if (_l.try_lock()) {
                *_addr = value;
            }
            _l.unlock()
        }

        T* get() {
            return _addr;
        }

        // Mutex

    protected:
        T * _addr;
        std::mutex _l;

};

template <class T=double>
class network_connection: connection {

    network_connection(T *addr, std::string ip, int port) {
        // Create UDP connection
        _addr = addr
    }

    void set(T value) {
        // Recive value from udp and set it on *addr
    }
}