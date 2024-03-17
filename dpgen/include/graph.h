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