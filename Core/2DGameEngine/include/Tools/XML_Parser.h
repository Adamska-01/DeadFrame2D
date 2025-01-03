#pragma once
#include <string>


template <typename T>
class XML_Parser
{
public:
	virtual T Parse(std::string source) = 0;
};