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

	for (int i = 0; i < lines.size(); i++)
	{
		std::string line = lines[i];
		std::string stripped_line = strip_comment_from_line(line);

		if (stripped_line.size() > 0)
		{
			if (stripped_line.find("=") == std::string::npos)
			{
				verilog_lines.push_back(create_port_declaration_from_line(stripped_line));
			}
			else
			{
				verilog_lines.push_back(create_module_instance_from_line(stripped_line, i));
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
* line_num: line number in netlist, used to create a unique module instance name.
* Returns a module instantiation written in Verilog.
*/
std::string create_module_instance_from_line(std::string line, int line_num)
{
	std::string veri_line = "";
	std::vector<std::string> split_line = split_string(line);

	// Determine the module to instantiate
	std::string module_type = determine_module(split_line);
	veri_line += module_type;

	// Generate a unique name for the instantiation of the module
	veri_line += " " + module_type[0] + std::to_string(line_num);

	// TODO: Write function to write inputs correctly

	return veri_line;
}

/*
* Determines which module should be instantiated based on a line from the netlist.
*
* split_line: a vector of strings containing a line split from a netlist file.
* Returns a string corresponding to the correct module.
*/
std::string determine_module(std::vector<std::string> split_line)
{
	if (split_line.size() < 4)
	{
		return "REG";
	}
	
	std::string op = split_line[3];
	
	if (op.compare("+") == 0)
	{
		return split_line[4].compare("1") == 0 ? "INC" : "ADD";
	}
	else if (op.compare("-") == 0)
	{
		return split_line[4].compare("1") == 0 ? "DEC" : "SUB";
	}
	else
	{
		auto it = std::find(OPERATIONS.begin(), OPERATIONS.end(), op);
		if (it != OPERATIONS.end())
		{
			int index = it - OPERATIONS.begin();
			return MODULES[index];
		}
	}
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