#pragma comment(linker, "/STACK:1000000000")
#include "Coding.h"
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <boost/bimap/support/lambda.hpp>
#include "BinaryString.h"
#include "const.h"

namespace HuffmanNs {
    class Vertex
    {
    public:
        char data = {};
        double freq = 0;
        Vertex* left = nullptr;
        Vertex* right = nullptr;

        Vertex(char data_, double freq_) {
            data = data_;
            freq = freq_;
        }
    };

    struct vertexByFreqComp
    {
        bool operator() (Vertex* left, Vertex* right) const {
            return left->freq > right->freq;
        }
    };

    void Huffman(std::multiset<Vertex*, vertexByFreqComp>& alphabet) {
        while (alphabet.size() > 1) {
            Vertex* mergedNode = new Vertex(char(), (*alphabet.rbegin())->freq + (*(++alphabet.rbegin()))->freq);

            mergedNode->right = *alphabet.rbegin();
            mergedNode->left = *(++alphabet.rbegin());

            alphabet.erase(--alphabet.end());
            alphabet.erase(--alphabet.end());

            alphabet.insert(mergedNode);
        }
    }

    void buildKeywords(Vertex* p, boost::bimap<char, std::string>& keywordBM, std::string keyword, char bit) {
        keyword.push_back(bit);

        if (p->left != nullptr && p->right != nullptr) {
            buildKeywords(p->left, keywordBM, keyword, '0');
            buildKeywords(p->right, keywordBM, keyword, '1');
        }
        else keywordBM.insert({ p->data, keyword });
    }

    void KeywordBuildingSetup(Vertex* p, boost::bimap<char, std::string>& keywordBM) {
        if (p != nullptr) {
            if (p->left != nullptr && p->right != nullptr) {
                buildKeywords(p->left, keywordBM, "", '0');
                buildKeywords(p->right, keywordBM, "", '1');
            }
        }
    }
}

namespace FanoNs {
    int getMedian(int L, int R, const std::vector<std::pair<char, double>>& alphabet, bool isRight) {
        double Sl = 0;
        double Sr = alphabet[R].second;
        int m = R;

        for (int i = L; i < R; i++) Sl += alphabet[i].second;

        while (Sl >= Sr) {
            m -= 1;
            Sl -= alphabet[m].second;
            Sr += alphabet[m].second;
        }

        return ((isRight && ((R - L) > 1)) ? (m - 1) : m);
    }

    int getMedianA2(int L, int R, const std::vector<std::pair<char, double>>& alphabet, bool isRight) {
        double probability = 0;
        double sum = 0;
        for (int i = L; i <= R; ++i) probability += alphabet[i].second;

        int i = 0;
        for (i = L; i < R; ++i) {
            if ((sum <= (probability / 2) && (sum + alphabet[i].second >= (probability / 2)))) {
                break;
            }
            sum += alphabet[i].second;
        }
        return i;
    }

    void Fano(int L, int R, const std::vector<std::pair<char, double>>& alphabet, std::map<char, std::string>& keywordM, bool isRight) {
        if (L < R) {
            int m = getMedianA2(L, R, alphabet, isRight);
            for (int i = L; i <= R; i++) (i <= m ? keywordM[alphabet[i].first] += '0' : keywordM[alphabet[i].first] += '1');
            Fano(L, m, alphabet, keywordM, false);
            Fano(m + 1, R, alphabet, keywordM, true);
        }
    }
}

bool Coding::alphabetCompValue(const std::pair<char, double>& a, const std::pair<char, double>& b) {
    return a.second > b.second;
}

bool Coding::alphabetCompKey(const std::pair<char, double>& a, const std::pair<char, double>& b) {
    return a.first <= b.first;
}

bool Coding::alphabetCompKeyR(const std::pair<char, double>& a, const std::pair<char, double>& b) {
    return a.first > b.first;
}

void DisplayTable(const std::vector<std::pair<char, double>>& alphabet, const boost::bimap<char, std::string>& keywordBM) {
    std::cout << std::endl;

    int maxKeywordLenght = 0;
    for (auto& el : keywordBM.left)
        if (static_cast<int>(el.second.size()) > maxKeywordLenght) maxKeywordLenght = static_cast<int>(el.second.size());
    maxKeywordLenght = (maxKeywordLenght > 8 ? maxKeywordLenght : 8);

    int numberWidth = 0;
    int tmp = maxKeywordLenght;
    for (numberWidth = 1; (tmp /= 10); numberWidth++);


    std::stringstream tablecout;
    tablecout.setf(std::ios::fixed);
    tablecout << std::setprecision(DOUBLE_PRECISION);

    tablecout << std::string(static_cast<long long>(DOUBLE_PRECISION + 2) + static_cast<long long>(numberWidth) +
        static_cast<long long>(maxKeywordLenght) + static_cast<long long>(3 * 4) + 2, '-') << std::endl;

    tablecout <<
        "| " <<
        std::setw(1) << "a" << " | " <<
        std::setw(DOUBLE_PRECISION + 2) << "Pi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " <<
        std::setw(numberWidth) << "L" << " | " <<
        std::setw(maxKeywordLenght) << "Keyword" << " | " << std::endl;

    tablecout << std::string(static_cast<long long>(DOUBLE_PRECISION + 2) + static_cast<long long>(numberWidth) +
        static_cast<long long>(maxKeywordLenght) + static_cast<long long>(3 * 4) + 2, '-') << std::endl;

    for (size_t i = 0; i < alphabet.size(); i++) {
        tablecout <<
            "| " <<
            std::setw(1) << alphabet[i].first << " | " <<
            std::setw(DOUBLE_PRECISION + 2) << alphabet[i].second << " | " <<
            std::setw(numberWidth) << keywordBM.left.at(alphabet[i].first).size() << " | " <<
            std::setw(maxKeywordLenght) << keywordBM.left.at(alphabet[i].first) << " | " << std::endl;
    }

    tablecout << std::string(static_cast<long long>(DOUBLE_PRECISION + 2) + static_cast<long long>(numberWidth) +
        static_cast<long long>(maxKeywordLenght) + static_cast<long long>(3 * 4) + 2, '-') << std::endl;

    std::cout << tablecout.str() << std::endl;
}

boost::bimap<char, std::string> Coding::CreateShennonBM(std::vector<std::pair<char, double>> alphabet)
{
    // Сортируем по убыванию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // Вектор комулятивных вероятностей
    std::vector<double> Q(alphabet.size() + 1, 0);

    // Вектор комулятивных вероятностей в двоичном виде
    std::vector<std::string> Qbinary(alphabet.size(), "");

    // Вектор кодовых слов
    std::vector<std::string> keyword(alphabet.size(), "");

    // Создаём словарь bimap для хранения пар 'Символ' : "Кодовое слово"
    boost::bimap<char, std::string> keywordBM;

    // Расчитываем комулятивные вероятности
    // Переводим их в двоичный вид
    // Вычисляем кодовые слова как ceil(-log2(alphabet[i])) количество символов в массив
    // кумулятивных вероятностей в двоичном виде после запятой
    for (size_t i = 1; i < Q.size(); i++) {
        Q[i] = (alphabet[i - (size_t)1].second + Q[i - (size_t)1]);
        Qbinary[i-1] = std::get<1>(BinaryString::ftobs(Q[i-1]));
        keyword[i-1] = Qbinary[i-1].substr(0, static_cast<int>(ceil(-log2(alphabet[i - 1].second))));

        keywordBM.insert({ alphabet[i - 1].first, keyword[i - 1] });
    }

    // Проверка на уникальность кодовых слов
    if (keywordBM.size() != keyword.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keywords are not unique!" << std::endl << std::endl;
        exit(15);
    }

    // Выводим таблицу кодовых слов
    std::cout << "Shennon Table" << std::endl;
    DisplayTable(alphabet, keywordBM);

    return keywordBM;
}

boost::bimap<char, std::string> Coding::CreateHuffmanBM(std::vector<std::pair<char, double>> alphabet)
{

    // Создаём словарь bimap для хранения пар 'Символ' : "Кодовое слово"
    boost::bimap<char, std::string> keywordBM;
    
    // Мультисет для хранения узлов дерева
    std::multiset<HuffmanNs::Vertex*, HuffmanNs::vertexByFreqComp> alphabetTree;

    for (const auto& letter : alphabet) alphabetTree.insert(new HuffmanNs::Vertex(letter.first, letter.second));

    HuffmanNs::Huffman(alphabetTree);

    HuffmanNs::KeywordBuildingSetup((*alphabetTree.begin()), keywordBM);

    // Проверка на уникальность кодовых слов
    if (keywordBM.size() != alphabet.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keywords are not unique!" << std::endl << std::endl;
        exit(15);
    }

    //sort(alphabet.begin(), alphabet.end(), alphabetCompKey);
    sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    std::cout << "Huffman Table" << std::endl;
    DisplayTable(alphabet, keywordBM);

    return keywordBM;
}

boost::bimap<char, std::string> Coding::CreateFanoBM(std::vector<std::pair<char, double>> alphabet)
{
    // Сортируем по убыванию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // Создаём словарь bimap для хранения пар 'Символ' : "Кодовое слово"
    boost::bimap<char, std::string> keywordBM;

    std::map<char, std::string> keywordM;

    for (auto& el : alphabet)
        keywordM.insert({ el.first, "" });

    FanoNs::Fano(0, static_cast<int>(alphabet.size() - 1), alphabet, keywordM, false);

    for (auto& el : alphabet) { keywordBM.insert({ el.first, keywordM.at(el.first) }); }

    // Проверка на уникальность кодовых слов
    if (keywordBM.size() != alphabet.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keywords are not unique!" << std::endl << std::endl;
        exit(15);
    }

    std::cout << "Fano Table" << std::endl;
    DisplayTable(alphabet, keywordBM);

    return keywordBM;
}

boost::bimap<char, std::string> Coding::CreateGilbertMooreBM(std::vector<std::pair<char, double>> alphabet)
{
    // Сортируем по убыванию ключей алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompKey);

    // Вектор комулятивных вероятностей
    std::vector<double> Q(alphabet.size(), 0);

    // Вектор комулятивных вероятностей в двоичном виде
    std::vector<std::string> Qbinary(alphabet.size(), "");

    // Вектор кодовых слов
    std::vector<std::string> keyword(alphabet.size(), "");

    // Создаём словарь bimap для хранения пар 'Символ' : "Кодовое слово"
    boost::bimap<char, std::string> keywordBM;

    // Расчитываем комулятивные вероятности
    // Переводим их в двоичный вид
    // Вычисляем кодовые слова как ceil(-log2(alphabet[i])) количество символов в массив
    // кумулятивных вероятностей в двоичном виде после запятой
    for (size_t i = 0; i < alphabet.size(); i++) {
        for (size_t j = 0; j < i; j++) Q[i] += alphabet[j].second;
        Q[i] += alphabet[i].second * 0.5f;
        Qbinary[i] = std::get<1>(BinaryString::ftobs(Q[i]));
        keyword[i] = Qbinary[i].substr(0, static_cast<size_t>((ceil(-log2(alphabet[i].second))) + 1));

        keywordBM.insert({ alphabet[i].first, keyword[i] });
    }

    // Проверка на уникальность кодовых слов
    if (keywordBM.size() != keyword.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
        exit(15);
    }

    std::cout << "GilbertMoore Table" << std::endl;
    DisplayTable(alphabet, keywordBM);

    return keywordBM;
}
