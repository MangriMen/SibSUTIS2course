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
	Lab11(std::string text, int type);

	void Init(std::string text, int type);

	std::string CodeBy(Code type, std::string text);
	std::string DecodeBy(Code type, std::string text);
	double GetAverageKeywordLenght(Code type);

	static std::vector<std::pair<char, double>> GetAlphabet(std::string text);
	double GetAlphabetEntropy();
	const std::string& GetText();
};

