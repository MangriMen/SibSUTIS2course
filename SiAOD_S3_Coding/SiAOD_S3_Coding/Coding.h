#pragma once
#include <string>
#include <boost/bimap.hpp>

class Coding
{
protected:
	static bool alphabetCompValue(const std::pair<char, double>& a, const std::pair<char, double>& b);

	static bool alphabetCompKey(const std::pair<char, double>& a, const std::pair<char, double>& b);

	static bool alphabetCompKeyR(const std::pair<char, double>& a, const std::pair<char, double>& b);
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

