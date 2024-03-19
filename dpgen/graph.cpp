#include "graph.h"
#include "Operation.h"
#include <vector>
#include <iostream>

double get_weight(Operation op) {
    std::string name = op.get_name().substr(0, op.get_name().find_first_of("><+"));
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
        nodes.push_back(Node(ops[i], get_weight(ops[i]), i));
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

bool find_id(std::vector<Node>::iterator start, std::vector<Node>::iterator end, int id) {
    while (start != end) {
        if ((*start).id == id) {
            return true;
        }
        start++;
    }
    return false;
}

std::vector<Node> do_topological_sort(std::vector<Node> graph)
{
    int num_nodes = graph.size();
    // Sort nodes by their dependencies (this should technically be a stack but eh)
    std::vector<Node> sorted_nodes;

    // First, get all nodes that don't have inputs
    for (Node& current_node : graph)
    {
        if (current_node.inputs.size() == 0)
        {
            std::cout << current_node.component.get_name() << "\n";
            sorted_nodes.push_back(current_node);
        }
    }

    // Then, add nodes to sorted_nodes as long as all the node's inputs are covered
    bool is_not_dependent = true;
    while (sorted_nodes.size() != num_nodes)
    {
        for (Node& current_node : graph)
        {
            if (find_id(sorted_nodes.begin(), sorted_nodes.end(), current_node.id))
            {
                continue;
            }
            // Check that all inputs are in the sorted_nodes
            for (Node* in_p : current_node.inputs)
            {
                // If input node is not found in sorted_nodes, move on
                if (!find_id(sorted_nodes.begin(), sorted_nodes.end(), in_p->id))
                {
                    is_not_dependent = false;
                }
            }
            //std::cout << is_not_dependent << "\n";

            // If all input nodes are in sorted_node, then the current_node has no dependencies
            if (is_not_dependent) {
                std::cout << sorted_nodes.size() << "\n";
                sorted_nodes.push_back(current_node);
            }

            // Reset the dependency check flag
            is_not_dependent = true;
        }
    }

    return sorted_nodes;
}

float calculate_critical_path(std::vector<Node> graph)
{
    std::vector<Node> sorted_nodes = do_topological_sort(graph);
    std::vector<Node*> unsorted;

    for(int i = 0; i < sorted_nodes.size(); i++) {
        for(int j = 0; j < sorted_nodes.size(); j++) {
            if (sorted_nodes[j].id == i) {
                unsorted.push_back(&sorted_nodes[j]);
            }
        }
    }

    for (int i = 0; i < sorted_nodes.size(); i++) {
        for (int n = 0; n < sorted_nodes[i].inputs.size(); n++) {
            sorted_nodes[i].inputs[n] = unsorted[sorted_nodes[i].inputs[n]->id];
        }
        for (int n = 0; n < sorted_nodes[i].outputs.size(); n++) {
            sorted_nodes[i].outputs[n] = unsorted[sorted_nodes[i].outputs[n]->id];
        }
    }

    for (Node node : sorted_nodes)
    {
        std::cout << node.component.get_name() << ", " << node.id << "\n";
        std::cout << node.weight << "\n";
    }

    float max_weight = 0.0;
    float critical_path = 0.0;

    for (Node& current_node : sorted_nodes)
    {
        for (Node* input : current_node.inputs)
        {
            Node current_input = *input;
            if (current_input.weight > max_weight)
            {
                max_weight = current_input.weight;
            }
        }

        // TODO: PLEASE check that the dereferenced pointer works here, should overwrite old weight
        // Add the node's current weight to the largest input weight
        current_node.weight += max_weight;
    }

    std::cout << "\n";
    for (Node node : sorted_nodes)
    {
        std::cout << node.weight << "\n";
    }

    // Check the weights of REG nodes and terminal nodes (nodes without outputs)
    for (Node node : graph)
    {
        if (node.component.get_name().compare("REG") == 0 || node.outputs.size() == 0)
        {
            if (node.weight > critical_path)
            {
                critical_path = node.weight;
            }
        }
    }

    return critical_path;
}