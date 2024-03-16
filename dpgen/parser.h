#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Data 
{
private:
	std::string name;
	int datawidth;
	bool is_signed;
public:
	std::string get_name() { return name; }
	int get_datawidth() { return datawidth; }
	bool is_signed() { return is_signed; }
};

class Operation {
private:
	std::string name;
	std::vector<Data> inputs;
	Data output;
public:
	std::string get_name() { return name; }
	std::vector<Data> get_inputs() { return inputs; }
	Data get_outputs() { return output; }
};

static const std::vector<std::string> OPERATIONS =
std::vector<std::string>{ "*", ">", "<", "==", "?", ">>", "<<", "/", "%" };

static const std::vector<std::string> MODULES = 
std::vector<std::string>{ "MUL", "COMP>", "COMP<", "COMP==", "MUX2x1", "SHR", "SHL", "DIV", "MOD" };

std::vector<std::string> convert_lines_to_verilog(std::vector<std::string>, std::string filename);

std::string strip_comment_from_line(std::string line);

std::vector<std::string> remove_empty_lines(std::vector<std::string> lines);

std::string write_module_definition
(std::vector<std::string> inputs, std::vector<std::string> outputs, std::string circuit_name);

std::string create_port_declaration_from_line
(std::string line, std::vector<std::string>& inputs, std::vector<std::string>& outputs);

std::string create_module_instance_from_line(std::string line, int line_num);

std::string determine_module(std::vector<std::string>);

std::string write_input_list(std::vector<std::string> split_line, std::string module_type);

std::vector<std::string> split_string(std::string line);

#endif