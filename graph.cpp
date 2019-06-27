#include "node.h"
#include "graph.h"

using namespace edb;

graph::graph() {
    _n = 0;
}

int graph::add_node(double (*sigma)(double)) {
    _map.insert({node(_n, sigma), std::set<node>()});
    _n++;
    return 0;
}
