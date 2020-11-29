#include "Coding.h"
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>
#include <set>
#include <sstream>
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

bool Coding::alphabetCompValue(std::pair<char, double> a, std::pair<char, double> b) {
    return a.second > b.second;
}

bool Coding::alphabetCompKey(std::pair<char, double> a, std::pair<char, double> b) {
    return a.first < b.first;
}

bool Coding::alphabetCompKeyR(std::pair<char, double> a, std::pair<char, double> b) {
    return a.first > b.first;
}

void DisplayTable(const std::vector<std::pair<char, double>>& alphabet, const boost::bimap<char, std::string>& keywordBM) {
    std::cout << std::endl;

    int maxKeywordLenght = 0;
    for (auto& el : keywordBM.left)
        if (static_cast<int>(el.second.size()) > maxKeywordLenght) maxKeywordLenght = static_cast<int>(el.second.size());

    int numberWidth = 0;
    int tmp = maxKeywordLenght;
    for (numberWidth = 1; (tmp /= 10); numberWidth++);


    std::stringstream tablecout;
    tablecout.setf(std::ios::fixed);
    tablecout << std::setprecision(DOUBLE_PRECISION);


    // Таблица
    tablecout <<
        std::setw(1) << "a" << " | " <<
        std::setw(DOUBLE_PRECISION + 2) << "Pi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " <<
        std::setw(numberWidth) << "L" << " | " <<
        std::setw(maxKeywordLenght) << "Keyword" << std::endl;

    tablecout << std::string(
        static_cast<long long>(DOUBLE_PRECISION + 2) + 
        static_cast<long long>(numberWidth) +
        static_cast<long long>(maxKeywordLenght) +
        static_cast<long long>(3 * 4) +
        static_cast < long long>(1), '-') << std::endl;

    for (size_t i = 0; i < alphabet.size(); i++) {
        tablecout <<
            std::setw(1) << alphabet[i].first << " | " <<
            std::setw(DOUBLE_PRECISION + 2) << alphabet[i].second << " | " <<
            std::setw(numberWidth) << keywordBM.left.at(alphabet[i].first).size() << " | " <<
            std::setw(maxKeywordLenght) << keywordBM.left.at(alphabet[i].first) << std::endl;
    }

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

    sort(alphabet.begin(), alphabet.end(), alphabetCompKey);
    //sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

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

    Huffman(alphabetTree);

    KeywordBuildingSetup((*alphabetTree.begin()), keywordBM);

    // Проверка на уникальность кодовых слов
    if (keywordBM.size() != alphabet.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keywords are not unique!" << std::endl << std::endl;
        exit(15);
    }

    sort(alphabet.begin(), alphabet.end(), alphabetCompKey);
    //sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    std::cout << "Huffman Table" << std::endl;
    DisplayTable(alphabet, keywordBM);

    return keywordBM;
}

boost::bimap<char, std::string> Coding::CreateFanoBM(std::vector<std::pair<char, double>> alphabet)
{
    //// Сортируем по не возрастанию алфавит
    //sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // Расчитываем вероятности
    //for (size_t i = 0; i < alphabet.size(); i++) {
    //    alphabet[i].second /= text.size();
    //    P.push_back(alphabet[i].second);
    //}

    //for (int i = 0; i < P.size(); i++) {
    //    std::cout << P[i] << std::endl;
    //}

    //buildFano(0, P.size()-1, 0);


    //for (int i = 0; i < 100; i++) {
    //    for (int j = 0; j < 100; j++) {
    //        std::cout << C[i][j] << " ";
    //    }
    //    std::cout << std::endl;
    //}

    return boost::bimap<char, std::string>();
}

boost::bimap<char, std::string> Coding::CreateGilbertMooreBM(std::vector<std::pair<char, double>> alphabet)
{
    // Сортируем по убыванию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompKey);

    // Создаём вектор для комулятивных вероятностей
    std::vector<double> Q(alphabet.size() + 1, 0);

    // Расчитываем комулятивные вероятности
    for (size_t i = 1; i < Q.size(); i++) {
        Q[i] = 0;
        for (size_t j = 0; j < i; j++) {
            if (j != (i - 1)) Q[i] += alphabet[j].second;
            else Q[i] += alphabet[j].second * 0.5f;
        }
    }

    // Создаём вектор для комулятивных вероятностей,
    // содержащих дробную часть комулятивных вероятностей в двоичном виде
    std::vector<std::string> Qbinary(alphabet.size() + 1, "");

    // Заполняем переводя дробную часть комулятивных вероятностей
    // в двоичный вид
    for (size_t i = 0; i < Qbinary.size(); i++) Qbinary[i] = std::get<1>(BinaryString::ftobs(Q[i]));

    // Создаём вектор длин кодовых слов
    std::vector<int> L(alphabet.size(), 0);

    // Вычисляем длинны кодовых слов по формуле L[i] = ceil(-log2(alphabet[i]))
    for (size_t i = 0; i < alphabet.size(); i++) L[i] = static_cast<int>(ceil(-log2(alphabet[i].second))) + 1;

    // Создаём вектор кодовых слов
    std::vector<std::string> keyword(alphabet.size(), "");

    // Вычисляем кодовые слова как L[i] количество символов в векторе двоичных комулятивных
    // вероятностей с начала строки
    for (size_t i = 1; i < alphabet.size() + 1; i++) keyword[i - 1] = Qbinary[i].substr(0, L[i - 1]);

    // Создаём словарь bimap для хранения пар 'Символ' : "Кодовое слово"

    boost::bimap<char, std::string> keywordBM;

    // Заполняем символами из алфавита и кодовыми словами
    for (size_t i = 0; i < alphabet.size(); i++) keywordBM.insert({ alphabet[i].first, keyword[i] });

    // Проверка на уникальность кодовых слов
    if (keywordBM.size() != keyword.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
        exit(15);
    }

    sort(alphabet.begin(), alphabet.end(), alphabetCompKey);
    //sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    std::cout << "GilbertMoore Table" << std::endl;
    DisplayTable(alphabet, keywordBM);

    return keywordBM;
}
