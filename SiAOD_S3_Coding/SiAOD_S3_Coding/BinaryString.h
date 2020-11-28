#pragma once
#include <string>
#include <vector>
#include <tuple>

class BinaryString
{
public:
	static std::string deleteSpaces(std::string& s);
	static std::tuple<std::string, int> itobs(int number);
	static std::tuple<std::string, std::string> ftobs(double number);
	static std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);
};

