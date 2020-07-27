#pragma once
#include <thread>

class agent {
    public:
        std::thread start() {
            return std::thread(agent::call, this);
        }

    protected:
        virtual void run();

        

    private:
        void static call(agent *a) {
            a->run();
        }
};