#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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