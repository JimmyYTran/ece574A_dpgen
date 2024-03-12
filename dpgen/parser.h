#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> convert_lines_to_verilog(std::vector<std::string>);

std::string strip_comment_from_line(std::string line);

std::string create_port_declaration_from_line(std::string line);

std::string create_module_instance_from_line(std::string line);

std::vector<std::string> split_string(std::string line);

#endif