#pragma once
#include <thread>

class system {

    public: 
        std::thread start() {
            return std::thread(system::call, this);
        }

    protected:
        void virtual run();
    private:

        void static call(system *s) {
            s->run();
        }
};


