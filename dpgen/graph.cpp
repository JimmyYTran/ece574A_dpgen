#include "graph.h"
#include "Operation.h"
#include <vector>

float DELAYS[12][6] = { {2.616, 2.644, 2.879, 3.061, 3.602, 3.966},
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

double get_weight(Operation op) {
    std::string name = op.get_name();
    int width = op.get_output().get_datawidth();
    if (name == "COMP") {
        auto inputs = op.get_inputs();
        int max = 0;
        for (auto i : inputs) {
            if (i.get_datawidth() > max) {
                max = i.get_datawidth();
            }
        }
        width = max;
    }
    int i1 = 0;
    int i2 = 0;
    if (name == "REG") { i1 = 0; }
    if (name == "ADD") { i1 = 1; }
    if (name == "SUB") { i1 = 2; }
    if (name == "MUL") { i1 = 3; }
    if (name == "COMP") { i1 = 4; }
    if (name == "MUX2x1") { i1 = 5; }
    if (name == "SHR") { i1 = 6; }
    if (name == "SHL") { i1 = 7; }
    if (name == "DIV") { i1 = 8; }
    if (name == "MOD") { i1 = 9; }
    if (name == "INC") { i1 = 10; }
    if (name == "DEC") { i1 = 11; }
    if (width == 1) { i2 = 0; }
    if (width == 2) { i2 = 1; }
    if (width == 8) { i2 = 2; }
    if (width == 16) { i2 = 3; }
    if (width == 32) { i2 = 4; }
    if (width == 64) { i2 = 5; }

    return DELAYS[i1][i2];
}

std::vector<Node> create_graph(std::vector<Operation> ops) {
    std::vector<Node> nodes;

    for (int i = 0; i < ops.size(); i++) {
        nodes.push_back(Node(ops[i], get_weight(ops[i])));
    }

    for (int i1 = 0; i1 < ops.size(); i1++) {
        for (int i2 = 0; i2 < ops.size(); i2++) {
            if (i1 == i2) {
                continue;
            }
            std::string n1_name = nodes[i1].component.get_output().get_name();
            for(auto n : nodes[i2].component.get_inputs()) {
                if (n1_name == n.get_name()) {
                    nodes[i1].outputs.push_back(&nodes[i2]);
                    nodes[i2].inputs.push_back(&nodes[i1]);
                }
            }
        }
    }
    return nodes;
}

/*
std::vector<Node*> do_topological_sort(std::vector<Node> graph)
{
    int num_nodes = graph.size();
    // Sort nodes by their dependencies (this should technically be a stack but eh)
    std::vector<Node*> sorted_nodes;

    // First, get all nodes that don't have inputs
    for (Node current_node : graph)
    {
        if (current_node.inputs.size() == 0)
        {
            sorted_nodes.push_back(&current_node);
        }
    }

    // Then, add nodes to sorted_nodes as long as all the node's inputs are covered
    bool is_not_dependent = true;
    while (sorted_nodes.size() != num_nodes)
    {
        for (Node current_node : graph)
        {
            // Check that all inputs are in the sorted_nodes
            for (Node* in_p : current_node.inputs)
            {
                // If input node is not found in sorted_nodes, move on
                if (is_not_dependent &&
                    std::find(sorted_nodes.begin(), sorted_nodes.end(), in_p) == sorted_nodes.end())
                {
                    is_not_dependent = false;
                }
            }

            // If all input nodes are in sorted_node, then the current_node has no dependencies
            if (is_not_dependent) {
                sorted_nodes.push_back(&current_node);
            }

            // Reset the dependency check flag
            is_not_dependent = true;
        }
    }

    return sorted_nodes;
}

float calculate_critical_path(std::vector<Node> graph)
{
    std::vector<Node*> sorted_nodes = do_topological_sort(graph);

    float max_weight = 0.0;
    float critical_path = 0.0;

    for (Node* node_p : sorted_nodes)
    {
        Node current_Node = *node_p;

        for (Node* input : current_Node.inputs)
        {
            Node current_input = *input;
            if (current_input.weight > max_weight)
            {
                max_weight = current_input.weight;
            }
        }

        // TODO: PLEASE check that the dereferenced pointer works here, should overwrite old weight
        // Add the node's current weight to the largest input weight
        current_Node.weight += max_weight;
    }

    // TODO: Check nodes without outputs: node with largest weight will have the critical path
}
*/