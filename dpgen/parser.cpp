#include "parser.h"

/*
* Convert lines from a behavioral netlist to lines in structural Verilog.
* Additionally, keeps track of each operation and its inputs and outputs.
*
* lines: vector of strings containing lines read from a netlist file.
* filename: string containing the filename, used for creating a circuit name.
* operations: vector of Operations to keep track of each operation.
* Returns a vector of strings, with each string being a line of Verilog.
*/
std::vector<std::string> parse_netlist_lines
(std::vector<std::string> lines, std::string filename, std::vector<Operation>& operations)
{
	std::vector<std::string> verilog_lines;

	for (int i = 0; i < lines.size(); i++)
	{
		lines[i] = strip_comment_from_line(lines[i]);
	}

	lines = remove_empty_lines(lines);

	int line_index = 0;
	std::vector<Data> ports;

	// Write the port declarations, while also keeping track of inputs/outputs/wires
	while (line_index < lines.size() && lines[line_index].find("=") == std::string::npos)
	{
		verilog_lines.push_back("\t" + create_port_declaration_from_line(lines[line_index], ports));
		line_index++;
	}

	while (line_index < lines.size())
	{
		verilog_lines.push_back("\t" +
			create_module_instance_from_line(lines[line_index], line_index, ports, operations));
		line_index++;
	}

	verilog_lines.insert(verilog_lines.begin(), write_module_definition(ports, filename));
	verilog_lines.insert(verilog_lines.begin(), "`timescale 1ns / 1ps");
	verilog_lines.push_back("endmodule");

	return verilog_lines;
}

/*
* Removes comments from a line from a behavioral netlist.
*
* line: a string containing a line from a netlist file.
* Returns the string without comments.
*/
std::string strip_comment_from_line(std::string line)
{
	std::size_t found = line.find("//");
	return line.substr(0, found);
}

/*
* Removes empty lines (to catch any lines that were commented out).
*
* lines: vector of strings containing lines from a netlist.
* Returns the vector of strings with empty strings removed.
*/
std::vector<std::string> remove_empty_lines(std::vector<std::string> lines)
{
	std::vector<std::string> output_lines;

	for (std::string line : lines)
	{
		if (!line.empty())
		{
			output_lines.push_back(line);
		}
	}

	return output_lines;
}

std::string write_module_definition(std::vector<Data> ports, std::string circuit_name)
{
	Data current_port;
	std::string module_def = "module " + circuit_name + "(";

	for (int i = 0; i < ports.size(); i++)
	{
		current_port = ports[i];

		if (current_port.get_datatype().compare("input") == 0 ||
			current_port.get_datatype().compare("output") == 0)
		{
			module_def += current_port.get_name() + ", ";
		}
	}

	// Get rid of the last comma and end module header
	module_def = module_def.substr(0, module_def.find_last_of(",")) + ");";
	return module_def;
}

/*
* Creates a Verilog port declaration (input, output, wire) from a line from a netlist.
*
* line: a string containing a line from a netlist file.
* ports: a vector of Data containing inputs, outputs, and wires (used later).
* Returns a port declaration written in Verilog.
*/
std::string create_port_declaration_from_line(std::string line, std::vector<Data>& ports)
{
	std::string veri_line = "";
	std::vector<std::string> split_line = split_string(line);
	std::string datatype = "";
	int datawidth = 1;			// datawidth is 1 unless otherwise specified
	bool is_signed = false;		// port is unsigned unless otherwise specified
	std::string port_name = "";

	// Determine the type of port (input/output/wire)
	datatype = split_line[0];
	veri_line += datatype;

	// Determine if the port is signed
	if (split_line[1].at(0) != 'U')
	{
		is_signed = true;
	}

	// Determine the datawidth of ports defined on this line from the datatype
	std::size_t it = split_line[1].find_first_of("0123456789");
	if (it != std::string::npos)
	{
		datawidth = std::stoi(split_line[1].substr(it));
		veri_line += " [" + std::to_string(datawidth - 1) + ":0]";
	}

	// Appending the inputs/outputs/wires
	for (int i = 2; i < split_line.size(); i++)
	{
		veri_line += " " + split_line[i];

		port_name = split_line[i].substr(0, split_line[i].find(","));

		// Only add a new port if it hasn't already been added
		if (find_port(ports, port_name) == -1)
		{
			ports.push_back(Data(port_name, datatype, datawidth, is_signed));
		}
	}

	return veri_line + ";";
}

/*
* Creates an instance of a module in Verilog from a line from a netlist.
*
* line: a string containing a line from a netlist file.
* line_num: line number in netlist, used to create a unique module instance name.
* ports: vector of Data, containing all inputs/outputs/wires.
* operations: vector of Operations, for keeping track of modules and their inputs/outputs.
* Returns a module instantiation written in Verilog.
*/
std::string create_module_instance_from_line
(std::string line, int line_num, std::vector<Data> ports, std::vector<Operation>& operations)
{
	std::string veri_line = "";
	std::vector<std::string> split_line = split_string(line);

	// Determine the module to instantiate
	std::string module_type = determine_module(split_line);

	operations.push_back(parse_line_to_operation(split_line, module_type, ports));
	veri_line += module_type.substr(0, module_type.find_first_of("><+"));

	// Generate a unique name for the instantiation of the module
	veri_line += " " + module_type.substr(0, 1) + std::to_string(line_num);

	// Generate list of inputs/outputs/wires
	veri_line += write_input_list(split_line, module_type);

	veri_line += ";";
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
		auto it = std::find(OPERATION_SYMBOLS.begin(), OPERATION_SYMBOLS.end(), op);
		if (it != OPERATION_SYMBOLS.end())
		{
			int index = it - OPERATION_SYMBOLS.begin();
			return MODULES[index];
		}
		else
		{
			return "ERROR";
		}
	}
}

/*
* Create an Operation instance with inputs and outputs from a line split from a netlist.
*
* split_line: vector of strings from a netlist split line.
* module_type: string representing the operation type.
* ports: vector of Data containing all inputs and outputs for the netlist.
* Returns an Operation instance.
*/
Operation parse_line_to_operation
(std::vector<std::string> split_line, std::string module_type, std::vector<Data>& ports)
{
	// Construct a new operation from the module_type
	Operation new_op = Operation(module_type);
	int input_index;

	// Set the output for the new operation	
	int output_index = find_port(ports, split_line[0]);
	new_op.set_output(ports[output_index]);

	if (module_type.compare("REG") == 0)
	{
		input_index = find_port(ports, split_line[2]);
		new_op.add_input(ports[input_index]);

		// For Clk and Rst, need to add to both new operation and ports (so we can track all inputs)
		Data clk_input = Data("Clk", "input", 1, false);
		ports.push_back(clk_input);
		new_op.add_input(clk_input);
		Data rst_input = Data("Rst", "input", 1, false);
		ports.push_back(rst_input);
		new_op.add_input(rst_input);
	}
	else
	{
		for (int i = 2; i < split_line.size(); i = i + 2)
		{
			input_index = find_port(ports, split_line[i]);
			ports.push_back(ports[input_index]);
			new_op.add_input(ports[input_index]);
		}
	}

	return new_op;
}

/*
* Writes a list of inputs for the module.
*
* split_line: a vector of strings containing a line split from a netlist file.
* module_type: operation that the module performs, corresponding to MODULES.
* Returns a string for the input list.
*/
std::string write_input_list(std::vector<std::string> split_line, std::string module_type)
{
	std::string input_list = "(";

	if (module_type.compare("REG") == 0)
	{
		input_list += "Clk, Rst, " + split_line[2] + ", " + split_line[0];
	}
	else if (module_type.compare("COMP>") == 0)
	{
		input_list += split_line[2] + ", " + split_line[4] + ", " + split_line[0] + " _, _";
	}
	else if (module_type.compare("COMP<") == 0)
	{
		input_list += split_line[2] + ", " + split_line[4] + ", _," + split_line[0] + ", _";
	}
	else if (module_type.compare("COMP==") == 0)
	{
		input_list += split_line[2] + ", " + split_line[4] + " _, _, " + split_line[0];
	}
	else if (module_type.compare("MUX2x1") == 0)
	{
		input_list += split_line[2] + ", " + split_line[4] + ", " + split_line[6] + ", " + split_line[0];
	}
	else if (module_type.compare("INC") == 0 || module_type.compare("DEC") == 0)
	{
		input_list += split_line[2] + ", " + split_line[0];
	}
	else
	{
		input_list += split_line[2] + ", " + split_line[4] + ", " + split_line[0];
	}

	input_list += ")";
	return input_list;
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
		if (word.size() != 0)
		{
			split_line.push_back(word);
		}
	}

	return split_line;
}

/*
* Check if the port (input/output/wire) has already been tracked in the vector of ports.
*
* ports: a vector of Data corresponding to ports (inputs/outputs/wires) from a netlist.
* Returns the index of the port if found, else -1.
*/
int find_port(std::vector<Data> ports, std::string new_port_name)
{
	for (int i = 0; i < ports.size(); i++)
	{
		if (ports[i].get_name().compare(new_port_name) == 0)
		{
			return i;
		}
	}

	return -1;
}