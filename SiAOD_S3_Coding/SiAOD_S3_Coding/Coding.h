#pragma once
#include <string>
#include <boost/bimap.hpp>

class Coding
{
protected:
	static bool alphabetCompValue(std::pair<char, double> a, std::pair<char, double> b);

	static bool alphabetCompKeyR(std::pair<char, double> a, std::pair<char, double> b);
public:
	enum class Code
	{
		Shennon,
		Huffman,
		Fano,
		GilbertMoore
	};

	static boost::bimap<char, std::string> CreateShennonBM(std::vector<std::pair<char, double>> alphabet);

	static boost::bimap<char, std::string> CreateHuffmanBM(std::vector<std::pair<char, double>> alphabet);

	static boost::bimap<char, std::string> CreateFanoBM(std::vector<std::pair<char, double>> alphabet);

	static boost::bimap<char, std::string> CreateGilbertMooreBM(std::vector<std::pair<char, double>> alphabet);
};

