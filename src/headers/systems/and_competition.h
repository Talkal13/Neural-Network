#include "../system.h"
#include "../evolution/agent.h"
#include "../nn/layered_net.h"

#include <vector>
#include <thread>


class and_agent : agent {

    public:

		and_agent() {

		}

    private:
        void run() {

        }

};

class and_competition : system {
    public:
        and_competition(size_t initial) {
            _n = initial;
        }

    private:
        void run() {
            std::vector<std::thread> _thrs;
            for (size_t i = 0; i < _n; i++) {
                _ags.push_back(new and_agent());
                _thrs.push_back(_ags[i]->start());
            }

            train();

            while(true) {
            
            }

        }

        void train() {
            for (agent *a : _ags) {
                std::vector<bool *> in = a->get_inputs();
            }
        }

        std::vector<and_agent*> _ags;
        size_t _n;
};