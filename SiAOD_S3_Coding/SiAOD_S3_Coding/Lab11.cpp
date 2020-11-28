#include "Lab11.h"
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>
#include <map>
#include <sstream>
#include "BinaryString.h"
#include "const.h"

size_t search(std::vector<std::pair<char, double>>& pair, const char& el) {
    for (size_t i = 0; i < pair.size(); i++) if (pair[i].first == el) return i;
    return std::string::npos;
}

bool alphabetCompValue(std::pair<char, double> a, std::pair<char, double> b) {
    return a.second > b.second;
}

bool alphabetCompKey(std::pair<char, double> a, std::pair<char, double> b) {
    return a.first < b.first;
}

Lab11::Lab11() :
    ShennonBM(boost::bimap<char, std::string>()),
    HuffmanBM(boost::bimap<char, std::string>()),
    FanoBM(boost::bimap<char, std::string>()),
    GilbertMooreBM(boost::bimap<char, std::string>())
{
}

Lab11::Lab11(std::string text)
{
    Init(text);
}

Lab11::~Lab11()
{
    ShennonBM.clear();
    HuffmanBM.clear();
    FanoBM.clear();
    GilbertMooreBM.clear();
}

void Lab11::Init(std::string text)
{
    ShennonBM = CreateShennonBM(text);
    //ShennonBM = boost::bimap<char, std::string>();;
    //HuffmanBM = CreateHuffmanBM(text);
    //FanoBM = CreateFanoBM(text);
    GilbertMooreBM = CreateGilbertMooreBM(text);
    HuffmanBM = boost::bimap<char, std::string>();
    FanoBM = boost::bimap<char, std::string>();;
    //GilbertMooreBM = boost::bimap<char, std::string>();
}

template< class MapType >
void print_map(const MapType& m)
{
    typedef typename MapType::const_iterator const_iterator;
    for (const_iterator iter = m.begin(), iend = m.end(); iter != iend; ++iter)
    {
        std::cout << iter->first << "-->" << iter->second << std::endl;
    }
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

    boost::bimap<char, std::string>::right_map* decodeMapType = nullptr;

    switch (type)
    {
    case Lab11::Code::Shennon:
        decodeMapType = &ShennonBM.right;
        break;
    case Lab11::Code::Huffman:
        //decodeMapType = &HuffmanBM.right;
        break;
    case Lab11::Code::Fano:
        //decodeMapType = &FanoBM.right;
        break;
    case Lab11::Code::GilbertMoore:
        decodeMapType = &GilbertMooreBM.right;
        break;
    default:
        return out;
        break;
    }

    if (decodeMapType == nullptr) exit(-1000);

    boost::bimap<char, std::string>::right_map &decodeMap = *decodeMapType;

    decodeMapType = nullptr;

    auto found = decodeMap.end();

    size_t i = 0;
    while (i < text.size()) {
        tmp.clear();
        found = decodeMap.end();

        do {
            tmp += text[i++];
            found = decodeMap.find(tmp);
        } while (i < text.size() && found == decodeMap.end());

        if (found != decodeMap.end()) out += found->second;
    }

    for (int i = static_cast<int>(out.size()) - 1; i >= 0; i--)
        if (out[i] != ' ') { out.resize(out.size() - (out.size() - i - 1)); break; }

    return out;
}

void ShennonTable(const std::vector<int>& L, const std::vector<std::pair<char, double>>& alphabet, const std::vector<double>& Q, const std::vector<std::string>& keyword) {
    std::cout << std::endl;

    std::cout << "Shennon table" << std::endl << std::endl;

    int numberWidth = 0;
    int tmp = L.back();
    for (numberWidth = 1; (tmp /= 10); numberWidth++);

    std::stringstream tablecout;
    tablecout.setf(std::ios::fixed);
    tablecout << std::setprecision(DOUBLE_PRECISION);

    // Таблица
    tablecout << std::setw(1) << "a" << " | " << std::setw(DOUBLE_PRECISION + 2) << "Pi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " << std::setw(DOUBLE_PRECISION + 2) << "Qi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " << std::setw(numberWidth) << "L" << " | " << std::setw(keyword.back().size()) << "Keyword" << std::endl;
    tablecout << std::string(64, '—') << std::endl;
    for (size_t i = 0; i < alphabet.size(); i++) {
        tablecout << std::setw(1) << alphabet[i].first << " | " << std::setw(DOUBLE_PRECISION + 2) << alphabet[i].second << " | " << std::setw(DOUBLE_PRECISION) << Q[i+1] << " | " << std::setw(numberWidth) << L[i] << " | " << std::setw(keyword.back().size()) << keyword[i] << std::endl;
    }

    std::cout << tablecout.str() << std::endl;
}

void GilbertMooreTable(const std::vector<int>& L, const std::vector<std::pair<char, double>>& alphabet, const std::vector<double>& Q, const std::vector<std::string>& keyword) {
    std::cout << std::endl;

    std::cout << "GilbertMoore table" << std::endl << std::endl;

    int numberWidth = 0;
    int tmp = L.back();
    for (numberWidth = 1; (tmp /= 10); numberWidth++);

    std::stringstream tablecout;
    tablecout.setf(std::ios::fixed);
    tablecout << std::setprecision(DOUBLE_PRECISION);

    // Таблица
    tablecout << std::setw(1) << "a" << " | " << std::setw(DOUBLE_PRECISION + 2) << "Pi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " << std::setw(DOUBLE_PRECISION + 2) << "Qi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " << std::setw(numberWidth) << "L" << " | " << std::setw(keyword.back().size()) << "Keyword" << std::endl;
    tablecout << std::string(64, '—') << std::endl;
    for (size_t i = 0; i < alphabet.size(); i++) {
        tablecout << std::setw(1) << alphabet[i].first << " | " << std::setw(DOUBLE_PRECISION + 2) << alphabet[i].second << " | " << std::setw(DOUBLE_PRECISION) << Q[i + 1] << " | " << std::setw(numberWidth) << L[i] << " | " << std::setw(keyword.back().size()) << keyword[i] << std::endl;
    }

    std::cout << tablecout.str() << std::endl;
}


boost::bimap<char, std::string> Lab11::CreateShennonBM(std::string text)
{
    // Создаём вектор пар для букв алфавита и их вероятностей
    std::vector<std::pair<char, double>> alphabet;

    // Заполняем уникальными буквами и частотой встречи её в тексте
    for (size_t i = 0; i < text.size(); i++) {
        size_t index = search(alphabet, text[i]);

        if (index != std::string::npos) alphabet[index].second += 1.0;
        else alphabet.push_back(std::make_pair(text[i], 1.0));
    }

    // Сортируем по не возрастанию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // Расчитываем вероятности
    for (size_t i = 0; i < alphabet.size(); i++) alphabet[i].second /= text.size();

    // Создаём вектор для комулятивных вероятностей
    std::vector<double> Q(alphabet.size() + 1, 0);

    // Расчитываем комулятивные вероятности
    for (size_t i = 1; i < Q.size(); i++) Q[i] = (alphabet[i - (size_t)1].second + Q[i - (size_t)1]);

    // Создаём вектор для комулятивных вероятностей,
    // содержащих дробную часть комулятивных вероятностей в двоичном виде
    std::vector<std::string> Qbinary(alphabet.size(), "");

    // Заполняем переводя дробную часть комулятивных вероятностей
    // в двоичный вид
    for (size_t i = 0; i < Qbinary.size(); i++) Qbinary[i] = std::get<1>(BinaryString::ftobs(Q[i]));

    // Создаём вектор длин кодовых слов
    std::vector<int> L(alphabet.size(), 0);

    // Вычисляем длинны кодовых слов по формуле L[i] = ceil(-log2(alphabet[i]))
    for (size_t i = 0; i < alphabet.size(); i++) L[i] = static_cast<int>(ceil(-log2(alphabet[i].second)));

    // Создаём вектор кодовых слов
    std::vector<std::string> keyword(alphabet.size(), "");

    // Вычисляем кодовые слова как L[i] количество символов в векторе двоичных комулятивных
    // вероятностей с начала строки
    for (size_t i = 0; i < alphabet.size(); i++) keyword[i] = Qbinary[i].substr(0, L[i]);

    ShennonTable(L, alphabet, Q, keyword);

    // Создаём словарь map для хранения пар 'Символ' : "Кодовое слово"

    boost::bimap<char, std::string> keywordBM;

    // Заполняем символами из алфавита и кодовыми словами
    for (size_t i = 0; i < alphabet.size(); i++) {
        keywordBM.insert({ alphabet[i].first, keyword[i] });
    }

    if (keywordBM.size() != keyword.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
        exit(15);
    }

    return keywordBM;
}

double* P = nullptr;

int* Len = nullptr;

int** CH = new int*[100];

int Up(int n, double q) {
    int j = 0;
    for (int i = (n - 1); i > 2; i--) {
        if (P[i - 1] <= q) {
            P[i] = P[i - 1];
        }
        else {
            j = i;
        }
    }

    P[j] = q;
    return j;
}

void Down(int n, int j) {
    int S = *CH[j];

    int L = Len[j];
    for (int i = j; i < n - 2; j--) {
        CH[i] = CH[i + 1];
        Len[i] = Len[i + 1];
    }
    CH[n - 1] = &S;
    CH[n] = &S;
    CH[n - 1][L + 1] = 0;
    CH[n][L + 1] = 1;
    Len[n - 1] = L + 1;
    Len[n] = L + 1;

}

void buildHuffman(int n, double* P) {
    if (n == 2) {
        CH[1][1] = 0;
        Len[1] = 1;
        CH[2][1] = 1;
        Len[2] = 1;
    }
    else {
        double q = P[n - 1] + P[n];
        int j = Up(n, q);
        buildHuffman(n - 1, P);
        Down(n, j);
    }
}

boost::bimap<char, std::string> Lab11::CreateHuffmanBM(std::string text)
{
    // Создаём вектор пар для букв алфавита и их вероятностей
    std::vector<std::pair<char, double>> alphabet;

    // Заполняем уникальными буквами и частотой встречи её в тексте
    for (size_t i = 0; i < text.size(); i++) {
        size_t index = search(alphabet, text[i]);

        if (index != std::string::npos) alphabet[index].second += 1.0;
        else alphabet.push_back(std::make_pair(text[i], 1.0));
    }

    // Сортируем по не возрастанию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // Расчитываем вероятности
    P = new double[alphabet.size()];
    Len = new int[alphabet.size()];
    for (int i = 0; i < alphabet.size(); i++) Len[i] = 0;
    for (size_t i = 0; i < alphabet.size(); i++) {
        alphabet[i].second /= text.size();
        P[i] = alphabet[i].second;
    }

    for (int i = 0; i < alphabet.size(); i++) {
        std::cout << P[i] << std::endl;
    }

    for (int i = 0; i < 100; i++) {
        CH[i] = new int[100];
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            CH[i][j] = 0;
        }
    }

    buildHuffman(static_cast<int>(alphabet.size()), P);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            std::cout << CH[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Создаём словарь map для хранения пар 'Символ' : "Кодовое слово"

    boost::bimap<char, std::string> keywordBM;

    //// Заполняем символами из алфавита и кодовыми словами
    //for (size_t i = 0; i < alphabet.size(); i++) {
    //    keywordBM.insert({ alphabet[i].first, keyword[i] });
    //}

    //if (keywordBM.size() != keyword.size()) {
    //    std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
    //    exit(15);
    //}

    return keywordBM;
}

std::vector<int> Lenght(100, 0);

std::vector<std::vector<int>> C(1000, std::vector<int>(1000, 0));

int getMedian(int L, int R) {
    //double sL = 0;
    //for (int i = 0; i < R - 1; i++) {
    //    sL = sL + P[i];
    //}
    //double sR = P[R];
    //int m = R;
    //while (sL >= sR) {
    //    m = m - 1;
    //    sL = sL - P[m];
    //    sR = sR + P[m];
    //}

    //return m;
    return 0;
}

void buildFano(int L, int R, int k) {
    //if (L < R) {
    //    k = k+1;
    //    int m = getMedian(L, R);
    //    for (int i = L; i < R; i++) {
    //        if (i <= m) { 
    //            C[i][k] = 0;
    //            Lenght[i] = Lenght[i] + 1;
    //        }
    //        else {
    //            C[i][k] = 1;
    //            Lenght[i] = Lenght[i] + 1;
    //        }
    //    }
    //    buildFano(L, m, k);
    //    buildFano(m+1, R, k);
    //}
}

boost::bimap<char, std::string> Lab11::CreateFanoBM(std::string text)
{
    // Создаём вектор пар для букв алфавита и их вероятностей
    std::vector<std::pair<char, double>> alphabet;

    // Заполняем уникальными буквами и частотой встречи её в тексте
    for (size_t i = 0; i < text.size(); i++) {
        size_t index = search(alphabet, text[i]);

        if (index != std::string::npos) alphabet[index].second += 1.0;
        else alphabet.push_back(std::make_pair(text[i], 1.0));
    }

    // Сортируем по не возрастанию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // Расчитываем вероятности
    //for (size_t i = 0; i < alphabet.size(); i++) {
    //    alphabet[i].second /= text.size();
    //    P.push_back(alphabet[i].second);
    //}

    //for (int i = 0; i < P.size(); i++) {
    //    std::cout << P[i] << std::endl;
    //}

    //buildFano(0, P.size()-1, 0);


    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            std::cout << C[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return boost::bimap<char, std::string>();
}

boost::bimap<char, std::string> Lab11::CreateGilbertMooreBM(std::string text)
{
    // Создаём вектор пар для букв алфавита и их вероятностей
    std::vector<std::pair<char, double>> alphabet;

    // Заполняем уникальными буквами и частотой встречи её в тексте
    for (size_t i = 0; i < text.size(); i++) {
        size_t index = search(alphabet, text[i]);

        if (index != std::string::npos) alphabet[index].second += 1.0;
        else alphabet.push_back(std::make_pair(text[i], 1.0));
    }

    // Сортируем по не возрастанию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetCompKey);

    // Расчитываем вероятности
    for (size_t i = 0; i < alphabet.size(); i++) alphabet[i].second /= text.size();

    // Создаём вектор для комулятивных вероятностей
    std::vector<double> Q(alphabet.size() + 1, 0);

    // Расчитываем комулятивные вероятности
    for (size_t i = 1; i < Q.size(); i++) {
        Q[i] = 0;
        for (size_t j = 0; j < i; j++) {
            if (j != (i-1)) Q[i] += alphabet[j].second;
            else Q[i] += alphabet[j].second * 0.5f;
        }
    }

    // Создаём вектор для комулятивных вероятностей,
    // содержащих дробную часть комулятивных вероятностей в двоичном виде
    std::vector<std::string> Qbinary(alphabet.size()+1, "");

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
    for (size_t i = 1; i < alphabet.size()+1; i++) keyword[i-1] = Qbinary[i].substr(0, L[i-1]);

    GilbertMooreTable(L, alphabet, Q, keyword);

    // Создаём словарь map для хранения пар 'Символ' : "Кодовое слово"

    boost::bimap<char, std::string> keywordBM;

    // Заполняем символами из алфавита и кодовыми словами
    for (size_t i = 0; i < alphabet.size(); i++) {
        keywordBM.insert({ alphabet[i].first, keyword[i] });
    }

    if (keywordBM.size() != keyword.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
        exit(15);
    }

    return keywordBM;
}
