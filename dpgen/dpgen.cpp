#include "fileio.h"
#include "parser.h"
#include "graph.h"
#include <iomanip>

int main(int argc, char* argv[])
{
	
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " netlistFile verilogFile" << std::endl;
		return 1;
	}

	std::string netlist_File = argv[1];
	std::string verilog_File = argv[2];

	std::vector<std::string> file_lines = read_file_to_strings(netlist_File);

	std::vector<Operation> operations;
	std::vector<std::string> output_lines = parse_netlist_lines(file_lines, netlist_File, operations);

	write_strings_to_file(output_lines, verilog_File);

	std::vector<Node> netlist_graph = create_graph(operations);
	
	/*
	std::string filename = "ucircuit1.txt";
	std::vector<std::string> file_lines = read_file_to_strings(filename);

	std::vector<Operation> operations;
	std::vector<std::string> output_lines = parse_netlist_lines(file_lines, filename, operations);

	write_strings_to_file(output_lines, "output_test.v");
	
	std::vector<Node> netlist_graph = create_graph(operations);

	
	std::cout << "Number of nodes: " << std::to_string(netlist_graph.size()) << "\n";

	for (Node node : netlist_graph)
	{
		std::cout << "Node: " << node.component.get_name() << "\n";

		for (Node* output : node.outputs)
		{
			Node next_op = (*output);
			std::cout << "\tNext operation is: " << next_op.component.get_name() << "\n";
		}
		
		for (Node* input : node.inputs)
		{
			Node prev_op = (*input);
			std::cout << "\tPrevious operation is: " << prev_op.component.get_name() << "\n";
			std::cout << "\t\tOutput for this is: " << prev_op.component.get_output().get_name() << "\n";
		}
	}
	*/

	float critical_path = calculate_critical_path(netlist_graph);

	//std::vector<Node*> sorted_nodes = do_topological_sort(netlist_graph);

	std::cout << std::fixed << std::setprecision(3);
	std::cout << "Critical Path : " << critical_path << " ns\n";

	return 0;
}

	/*
	For debugging purposes:
	for (Operation a_op : operations)
	{
		std::cout << a_op.get_name() << "\n";
		std::cout << "Output: " << a_op.get_output().get_name() << "\n";
		for (Data a_input : a_op.get_inputs())
		{
			std::cout << "Input: " << a_input.get_name() << "\n";
		}

		std::cout << "\n";
	}

	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
	std::cout << create_port_declaration_from_line("input UInt64 a, b, c, clk, r1, r2", inputs, outputs) << "\n";
	for (std::string input : inputs)
	{
		std::cout << input + "\n";
	}

	// std::cout << write_input_list(split_string("c = a + b"), "ADD") << "\n";
	std::cout << write_input_list(split_string("gEQz = g == zero"), "COMP==") << "\n";
	std::cout << write_input_list(split_string("zwire - gEQz ? e : f"), "MUX2x1") << "\n";
	std::cout << write_input_list(split_string("z = zwire"), "REG") << "\n";

	std::cout << create_module_instance_from_line("c = a + b", 1) << "\n";
	std::cout << create_module_instance_from_line("gEQz = g == zero", 2) << "\n";
	std::cout << create_module_instance_from_line("zwire - gEQz ? e : f", 3) << "\n";
	std::cout << create_module_instance_from_line("z = zwire", 4) << "\n";
	std::cout << create_module_instance_from_line("z = g << dLTe", 5) << "\n";
	*/
