#include "Data.h"

Data::Data()
{
	this->name = "";
	this->datawidth = 0;
	this->is_signed = true;
}

Data::Data(std::string name, int datawidth, bool is_signed)
{
	this->name = name;
	this->datawidth = datawidth;
	this->is_signed = is_signed;
}