#include "Lab11.h"
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>
#include <map>
#include <sstream>
#include "BinaryString.h"
#include "const.h"

static size_t search(std::vector<std::pair<char, double>>& pair, const char& el) {
    for (size_t i = 0; i < pair.size(); i++) if (pair[i].first == el) return i;
    return std::string::npos;
}

template< class MapType >
static void print_map(const MapType& m)
{
    typedef typename MapType::const_iterator const_iterator;
    for (const_iterator iter = m.begin(), iend = m.end(); iter != iend; ++iter)
    {
        std::cout << iter->first << "-->" << iter->second << std::endl;
    }
}

Lab11::Lab11() :
    ShennonBM(boost::bimap<char, std::string>()),
    HuffmanBM(boost::bimap<char, std::string>()),
    FanoBM(boost::bimap<char, std::string>()),
    GilbertMooreBM(boost::bimap<char, std::string>()),
    alphabet(std::vector<std::pair<char, double>>())
{
}

Lab11::Lab11(std::string text) :
    ShennonBM(boost::bimap<char, std::string>()),
    HuffmanBM(boost::bimap<char, std::string>()),
    FanoBM(boost::bimap<char, std::string>()),
    GilbertMooreBM(boost::bimap<char, std::string>()),
    alphabet(std::vector<std::pair<char, double>>()),
    originalText("")
{
    Init(text);
}

void Lab11::Init(std::string text)
{
    alphabet = GetAlphabet(text);
    originalText = text;

    //std::vector<std::pair<char, double>> test;

    //test.push_back(std::make_pair('1', 0.18));
    //test.push_back(std::make_pair('2', 0.18));
    //test.push_back(std::make_pair('3', 0.36));
    //test.push_back(std::make_pair('4', 0.07));
    //test.push_back(std::make_pair('5', 0.09));
    //test.push_back(std::make_pair('6', 0.12));

    //CreateGilbertMooreBM(test);

    ShennonBM = CreateShennonBM(alphabet);
    HuffmanBM = CreateHuffmanBM(alphabet);
    FanoBM = CreateFanoBM(alphabet);
    GilbertMooreBM = CreateGilbertMooreBM(alphabet);
}

std::string Lab11::CodeBy(Code type, std::string text)
{
    std::string out = "";
    switch (type)
    {
    case Lab11::Code::Shennon:
        for (const auto& ch : text) {
            try {
                out += ShennonBM.left.at(ch);
            }
            catch (std::out_of_range)
            {
                std::cerr << "[CODING]: The coding alphabet does not match the text alphabet";
                exit(-2);
            }
        }
        return out;
    case Lab11::Code::Huffman:
        for (const auto& ch : text) {
            try {
                out += HuffmanBM.left.at(ch);
            }
            catch (std::out_of_range)
            {
                std::cerr << "[CODING]: The coding alphabet does not match the text alphabet";
                exit(-2);
            }
        }
        break;
    case Lab11::Code::Fano:
        break;
    case Lab11::Code::GilbertMoore:
        for (const auto& ch : text) {
            try {
                out += GilbertMooreBM.left.at(ch);
            }
            catch (std::out_of_range)
            {
                std::cerr << "[CODING]: The coding alphabet does not match the text alphabet";
                exit(-2);
            }
        }
        break;
    default:
        break;
    }

    return out;
}

std::string Lab11::DecodeBy(Code type, std::string text)
{
    std::string out = "";
    std::string tmp = "";

    boost::bimap<char, std::string>::right_map* decodeMap = nullptr;

    switch (type)
    {
    case Lab11::Code::Shennon:
        decodeMap = &ShennonBM.right;
        break;
    case Lab11::Code::Huffman:
        decodeMap = &HuffmanBM.right;
        break;
    case Lab11::Code::Fano:
        //decodeMapType = &FanoBM.right;
        break;
    case Lab11::Code::GilbertMoore:
        decodeMap = &GilbertMooreBM.right;
        break;
    default:
        return out;
        break;
    }

    if (decodeMap == nullptr) exit(-1000);

    auto found = decodeMap->end();

    size_t i = 0;
    while (i < text.size()) {
        tmp.clear();
        found = decodeMap->end();

        do {
            tmp += text[i++];
            found = decodeMap->find(tmp);
        } while (i < text.size() && found == decodeMap->end());

        if (found != decodeMap->end()) out += found->second;
    }

    for (int i = static_cast<int>(out.size()) - 1; i >= 0; i--)
        if (out[i] != ' ') { out.resize(out.size() - (out.size() - i - 1)); break; }

    return out;
}

double Lab11::GetAverageKeywordLenght(Code type)
{
    double averageLenght = 0;

    boost::bimap<char, std::string>::left_map* mapType = nullptr;

    switch (type)
    {
    case Coding::Code::Shennon:
        mapType = &ShennonBM.left;
        break;
    case Coding::Code::Huffman:
        mapType = &HuffmanBM.left;
        break;
    case Coding::Code::Fano:
        mapType = &FanoBM.left;
        break;
    case Coding::Code::GilbertMoore:
        mapType = &GilbertMooreBM.left;
        break;
    default:
        break;
    }

    if (mapType == nullptr) exit(-1000);

    for (int i = 0; i < alphabet.size(); i++)
        averageLenght += alphabet[i].second * mapType->at(alphabet[i].first).size();
    
    return averageLenght;
}

std::vector<std::pair<char, double>> Lab11::GetAlphabet(std::string text)
{
    // Создаём вектор пар для букв алфавита и их вероятностей
    std::vector<std::pair<char, double>> alphabet;

    // Заполняем уникальными буквами и частотой встречи её в тексте
    for (size_t i = 0; i < text.size(); i++) {
        size_t index = search(alphabet, text[i]);

        if (index != std::string::npos) alphabet[index].second += 1.0;
        else alphabet.push_back(std::make_pair(text[i], 1.0));
    }

    // Расчитываем вероятности
    for (size_t i = 0; i < alphabet.size(); i++) alphabet[i].second /= text.size();

    // Сортируем по не возрастанию алфавит
    sort(alphabet.begin(), alphabet.end(), Coding::alphabetCompValue);

    return alphabet;
}

double Lab11::GetAlphabetEntropy()
{
    double entropy = 0;

    for (int i = 0; i < alphabet.size(); i++)
        entropy += -alphabet[i].second * log2(alphabet[i].second);

    return entropy;
}

const std::string& Lab11::GetText()
{
    return originalText;
}