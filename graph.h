#pragma once
#include <unordered_map>
#include <set>

#include "neuron.h"

namespace edb {
    class graph;
}

using edb::graph;
class graph {
    private:
        std::unordered_map<edb::node, std::set<edb::node>> _map;
        int _n = 0;
    public:

        graph();

        int add_node(double (*sigma)(double));

};
