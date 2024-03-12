#include "parser.h"

/*
* Convert lines from a behavioral netlist to lines in structural Verilog.
* 
* lines: vector of strings containing lines read from a netlist file
* Returns a vector of strings, with each string being a line of Verilog.
*/
std::vector<std::string> convert_lines_to_verilog(std::vector<std::string> lines)
{
	std::vector<std::string> verilog_lines;

	verilog_lines.push_back("`timescale 1ns / 1ps");

	for (std::string line : lines)
	{
		std::string stripped_line = strip_comment_from_line(line);

		if (stripped_line.size() > 0)
		{
			if (stripped_line.find("=") == std::string::npos)
			{
				verilog_lines.push_back(create_port_declaration_from_line(stripped_line));
			}
			else
			{
				verilog_lines.push_back(create_module_instance_from_line(stripped_line));
			}
		}
	}

	verilog_lines.push_back("endmodule");

	return verilog_lines;
}

/*
* Removes comments from a line from a behavioral netlist.
* 
* line: a string containing a line from a netlist file.
* Returns the same string without comments.
*/
std::string strip_comment_from_line(std::string line)
{
	std::size_t found = line.find("//");
	return line.substr(0, found);
}

/*
* Creates a Verilog port declaration (input, output, wire) from a line from a netlist.
* 
* line: a string containing a line from a netlist file.
* Returns a port declaration written in Verilog.
*/
std::string create_port_declaration_from_line(std::string line)
{
	std::string veri_line = "";
	std::vector<std::string> split_line = split_string(line);

	// Determine the type of port
	if (split_line[0].compare("input") == 0 ||
		split_line[0].compare("output") == 0 || 
		split_line[0].compare("wire") == 0)
	{
		veri_line += split_line[0];
	}
	else
	{
		return "";
	}

	// Determine the datawidth of ports defined on this line from the datatype
	std:size_t it = split_line[1].find_first_of("0123456789");
	if (it != std::string::npos)
	{
		int datawidth = std::stoi(split_line[1].substr(it)) - 1;
		veri_line += " [" + std::to_string(datawidth) + ":0]";
	}

	// Appending the inputs/outputs/wires
	for (int i = 2; i < split_line.size(); i++)
	{
		veri_line += " " + split_line[i];
	}

	return veri_line + ";";
}

/*
* Creates an instance of a module in Verilog from a line from a netlist.
* 
* line: a string containing a line from a netlist file.
* Returns a module instantiation written in Verilog.
*/
std::string create_module_instance_from_line(std::string line)
{
	std::string veri_line = "";
	std::vector<std::string> split_line = split_string(line);

	return veri_line;
}

/*
* Splits a line from a netlist, using a single space as the delimiter.
* 
* line: a string containing a line from a netlist file.
* Returns the result of the splitting the line by spaces as a vector of strings.
*/
std::vector<std::string> split_string(std::string line)
{
	std::vector<std::string> split_line;
	std::istringstream iss(line);
	std::string word;

	while (std::getline(iss, word, ' '))
	{
		split_line.push_back(word);
	}

	return split_line;
}