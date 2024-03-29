#ifndef GRAPH_H
#define GRAPH_H

#include "Operation.h"
#include <vector>

static const float DELAYS[12][6] = { {2.616, 2.644, 2.879, 3.061, 3.602, 3.966},
                                     {2.704, 3.713, 4.924, 5.638, 7.270, 9.566},
                                     {3.024, 3.412, 4.890, 5.569, 7.253, 9.566},
                                     {2.438, 3.651, 7.453, 7.811, 12.395, 15.354},
                                     {3.031, 3.934, 5.949, 6.256, 7.264, 8.416},
                                     {4.083, 4.115, 4.815, 5.623, 8.079, 8.766},
                                     {3.644, 4.007, 5.178, 6.460, 8.819, 11.095},
                                     {3.614, 3.980, 5.152, 6.549, 8.565, 11.220},
                                     {0.619, 2.144, 15.439,33.093, 86.312, 243.233},
                                     {0.758, 2.149, 16.078,35.563, 88.124, 250.583},
                                     {1.792, 2.218, 3.111, 3.471, 4.347, 6.200},
                                     {1.792, 2.218, 3.108, 3.701, 4.685, 6.503}
};

struct Node {
    Node(Operation c, double w, int i) {
        component = c;
        weight = w;
        id = i;
    } 
    Operation component;
    std::vector<Node*> inputs;
    std::vector<Node*> outputs;
    int id;
    double weight;
};

double get_weight(Operation op);

std::vector<Node> create_graph(std::vector<Operation> ops);

bool find_id(std::vector<Node>::iterator start, std::vector<Node>::iterator end, int id);

std::vector<Node> do_topological_sort(std::vector<Node> graph);

float calculate_critical_path(std::vector<Node> graph);

#endif