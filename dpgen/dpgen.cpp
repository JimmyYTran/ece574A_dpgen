#include "fileio.h"
#include "parser.h"
#include "graph.h"

int main(/*int argc, char* argv[]*/)
{
	/*
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " netlistFile verilogFile" << std::endl;
		return 1;
	}

	std::string netlist_File = argv[1];
	std::string verilog_File = argv[2];

	std::vector<std::string> file_lines = read_file_to_strings(netlist_File);

	std::vector<Operation> operations;
	std::vector<std::string> output_lines = parse_netlist_lines(file_lines, netlist_File, operations);

	for (std::string line : output_lines)
	{
		std::cout << line + "\n";
	}

	write_strings_to_file(output_lines, verilog_File);

	std::cout << "\n";

	std::vector<Node> netlist_graph = create_graph(operations);
	std::vector<Node*> sort_netlist_nodes = do_topological_sort(netlist_graph);
	float critical_path = calculate_critical_path(netlist_graph);

	std::string crit_path = std::to_string(critical_path);

	std::cout << "Critical Path : " << crit_path << " ns\n";

	*/

	std::vector<std::string> file_lines = read_file_to_strings("474a_circuit2.txt");

	std::vector<Operation> operations;
	std::vector<std::string> output_lines = parse_netlist_lines(file_lines, "474a_circuit2.txt", operations);

	write_strings_to_file(output_lines, "output_test.v");

	return 0;
}
