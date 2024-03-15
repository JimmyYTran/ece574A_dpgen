#include "fileio.h"
#include "parser.h"

int main(int argc, char *argv[])
{
	// TODO: Use argc and argv
	/*
	std::vector<std::string> file_lines = read_file_to_strings("474a_circuit1.txt");

	for (int i = 0; i < file_lines.size(); ++i)
	{
		std::vector<std::string> split_line = split_string(file_lines[i]);

		for (int j = 0; j < split_line.size(); j++)
		{
			std::cout << split_line[j] << "\n";
		}

		std::cout << "New line!\n\n";
	}
	*/

	//std::cout << create_port_declaration_from_line("wire UInt64 a, b, c, clk, r1, r2");

	std::cout << determine_module(split_string("d = a - 1"));

	return 0;
}