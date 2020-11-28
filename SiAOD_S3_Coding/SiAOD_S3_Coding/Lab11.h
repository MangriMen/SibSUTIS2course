#pragma once
#include <string>
#include <boost/bimap.hpp>

class Lab11
{
	boost::bimap<char, std::string> ShennonBM;
	boost::bimap<char, std::string> HuffmanBM;
	boost::bimap<char, std::string> FanoBM;
	boost::bimap<char, std::string> GilbertMooreBM;

	static boost::bimap<char, std::string> CreateShennonBM(std::string);
	static boost::bimap<char, std::string> CreateHuffmanBM(std::string text);
	static boost::bimap<char, std::string> CreateFanoBM(std::string text);
	static boost::bimap<char, std::string> CreateGilbertMooreBM(std::string text);
public:
	Lab11();
	Lab11(std::string text);

	~Lab11();

	void Init(std::string text);

	enum class Code
	{
		Shennon,
		Huffman,
		Fano,
		GilbertMoore
	};

	std::string CodeBy(Code type, std::string text);
	std::string DecodeBy(Code type, std::string text);
};

