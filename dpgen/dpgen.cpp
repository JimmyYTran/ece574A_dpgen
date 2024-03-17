#include "fileio.h"
#include "parser.h"

int main(int argc, char *argv[])
{
	// TODO: Use argc and argv
	std::vector<std::string> file_lines = read_file_to_strings("474a_circuit1.txt");

    std::vector<Operation> operations;
	std::vector<std::string> output_lines = parse_netlist_lines(file_lines, "Circuit1", operations);

	for (std::string line : output_lines)
	{
		std::cout << line + "\n";
	}
	
	/*
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
	std::cout << create_port_declaration_from_line("input UInt64 a, b, c, clk, r1, r2", inputs, outputs) << "\n";
	for (std::string input : inputs)
	{
		std::cout << input + "\n";
	}
	*/

	/*
	std::cout << write_input_list(split_string("c = a + b"), "ADD") << "\n";
	std::cout << write_input_list(split_string("gEQz = g == zero"), "COMP==") << "\n";
	std::cout << write_input_list(split_string("zwire - gEQz ? e : f"), "MUX2x1") << "\n";
	std::cout << write_input_list(split_string("z = zwire"), "REG") << "\n";
	*/

	/*
	std::cout << create_module_instance_from_line("c = a + b", 1) << "\n";
	std::cout << create_module_instance_from_line("gEQz = g == zero", 2) << "\n";
	std::cout << create_module_instance_from_line("zwire - gEQz ? e : f", 3) << "\n";
	std::cout << create_module_instance_from_line("z = zwire", 4) << "\n";
	std::cout << create_module_instance_from_line("z = g << dLTe", 5) << "\n";
	*/

	return 0;
}