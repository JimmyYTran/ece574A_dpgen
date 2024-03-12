#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> read_file_to_strings(std::string filename);

void write_strings_to_file(std::vector<std::string> strings, std::string filename);

#endif