#ifndef OPERATION_H
#define OPERATION_H

#include <vector>
#include "Data.h"

class Operation {
private:
	std::string name;
	std::vector<Data> inputs;
	Data output;
public:
	Operation(std::string name);

	std::string get_name() { return name; }
	std::vector<Data> get_inputs() { return inputs; }
	Data get_outputs() { return output; }
};

#endif