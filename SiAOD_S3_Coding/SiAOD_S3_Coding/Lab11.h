#pragma once
#include <string>
#include <boost/bimap.hpp>
#include "Coding.h"

class Lab11: public Coding
{
	std::string originalText;
	std::vector<std::pair<char, double>> alphabet;

	boost::bimap<char, std::string> ShennonBM;
	boost::bimap<char, std::string> HuffmanBM;
	boost::bimap<char, std::string> FanoBM;
	boost::bimap<char, std::string> GilbertMooreBM;

public:
	Lab11();
	Lab11(std::string text);

	void Init(std::string text);

	std::string CodeBy(Code type, std::string text);
	std::string DecodeBy(Code type, std::string text);

	static std::vector<std::pair<char, double>> GetAlphabet(std::string text);
	const std::string& GetText();
};

