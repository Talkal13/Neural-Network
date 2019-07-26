#include <thread>

class interface {
    public:
        std::thread start() {
            return std::thread(run, this);
        }
    protected:
        

        static void run(interface *in);

};