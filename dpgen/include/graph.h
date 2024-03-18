#ifndef GRAPH_H
#define GRAPH_H

#include "Operation.h"
#include <vector>

struct Node {
    Node(Operation c, double w) {
        component = c;
        weight = w;
    } 
    Operation component;
    std::vector<Node*> inputs;
    std::vector<Node*> outputs;
    double weight;
};

std::vector<Node> create_graph(std::vector<Operation> ops);

std::vector<Node*> do_topological_sort(std::vector<Node> graph);

float calculate_critical_path(std::vector<Node> graph);


#endif