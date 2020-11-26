#include <iostream>
#include <ctime>
#include <string>
#include <tuple>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <map>

constexpr auto DOUBLE_PRECISION = 16;

using namespace std;

enum class Coding
{
    FpV,
    Gamma,
    Omega
};

string deleteSpaces(string& s) {
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    return s;
}

tuple<string, int> itobs(int number) {
    string binaryNumber = "";
    int powerOfTwo = 0;

    if (number) {
        do {
            binaryNumber += to_string(number % 2);
            powerOfTwo++;
        } while (number /= 2);
        reverse(binaryNumber.begin(), binaryNumber.end());
    }

    return make_tuple(binaryNumber, powerOfTwo);
}

tuple<string, string> ftobs(double number) {
    int intPart = (int)(number);
    string intPartS = get<0>(itobs(intPart));
    double floatPart = number - intPart;
    string floatPartS = "";
    for (int i = 0; i < DOUBLE_PRECISION; i++) {
        floatPart = floatPart * 2;
        if (floatPart >= 1) {
            floatPartS += '1';
            floatPart -= 1;
        }
        else {
            floatPartS += '0';
        }
    }

    return make_tuple(intPartS, floatPartS);
}

vector<string>& split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string fixedVariable(int number) {
    tuple<string, int> convertedNum = itobs(number);

    string binaryNumber = get<0>(convertedNum);
    int power = get<1>(convertedNum);

    string keywordFirst = get<0>(itobs(power));
    string keywordSecond = "";

    if (power) keywordSecond = binaryNumber.substr(binaryNumber.length() - (static_cast<unsigned long long>(power) - 1), static_cast<unsigned long long>(power) - 1);

    return string(4 - keywordFirst.length(), '0') + keywordFirst + " " + keywordSecond;
}

string gammaCode(int number) {
    if (number) {
        tuple<string, int> convertedNum = itobs(number);
        string keywordFirst = string(static_cast<unsigned long long>(get<1>(convertedNum)) - 1, '0');
        string keywordSecond = get<0>(convertedNum);
        return keywordFirst + " " + keywordSecond;
    }
    
    return "";
}

void omegaCodeRec(string& word, int number) {
    if (number == 1) return;
    tuple<string, int> convertedNum = itobs(number);

    word.insert(0, get<0>(convertedNum) + " ");
    number = static_cast<int>(get<0>(convertedNum).length() - static_cast<size_t>(1));

    omegaCodeRec(word, number);
}

string omegaCode(int number) {
    if (!number) return "";

    string keyword = "0";
    omegaCodeRec(keyword, number);

    return keyword;
}

string RLE(string number, Coding type) {
    vector<string> tmpSeries;
    string outRLE = "";

    split(number, '1', tmpSeries);

    for (const string& ser : tmpSeries)
        switch (type)
        {
        case Coding::FpV:
            outRLE += fixedVariable(static_cast<int>(ser.length()));
            break;
        case Coding::Gamma:
            outRLE += gammaCode(static_cast<int>(ser.length()) + 1);
            break;
        case Coding::Omega:
            outRLE += omegaCode(static_cast<int>(ser.length()) + 1);
            break;
        default:
            cerr << endl << "[RLE]: Unknown coding type!" << endl;
            exit(-10);
            break;
        }

    return deleteSpaces(outRLE);
}

string RLE(int number, Coding type) {
    return RLE(get<0>(itobs(number)), type);
}

void createTab(int size) {
    int numberWidth = 0;
    int tmp = size;
    for (numberWidth = 0; (tmp /= 10); numberWidth++);

    cout << setw(numberWidth) << "Number" << "\t| Fixed + Variable | Alias gamma-code | Alias omega-code | RLE" << endl;
    for (int i = 0; i < size; ++i) {
        cout << setw(numberWidth) << i <<
            "\t| " << setw(16) << fixedVariable(i) << " | "<<
            setw(16) << gammaCode(i) << " | " << 
            setw(16) << omegaCode(i) << " | " <<
            setw(16) << RLE(i, Coding::Gamma) << 
            endl;
    }
    cout << endl;
}

void CodingTable() {
    int rows = 0;

    do {
        cout << "Enter the number of rows: ";
        cin >> rows;
        if (rows > 32600) cout << "Too big number, please" << endl;
    } while (rows > 32600);

    createTab(rows);
}

void writeBytesToFile(ofstream& file, const string& bitSequence) {
    bitset<CHAR_BIT> byte("00000000");
    size_t i = 0, b = 0;
    while (i < bitSequence.size()) {
        byte.reset();
        for (b = 0; (i < bitSequence.size()) && (b < CHAR_BIT); i++, b++) byte.set(CHAR_BIT - 1 - b, (bitSequence[i] == '1'));
        file << static_cast<uint_fast8_t>(byte.to_ulong());
    }
}

void readBytesFromFile(ifstream& file, string& bitSequence) {
    bitSequence.clear();
    while (!file.eof()) bitSequence += bitset<CHAR_BIT>(file.get()).to_string();
    bitSequence.resize(bitSequence.size() - 8);
}

void checkFileIsOpen(ofstream& file) {
    if (!file.is_open()) {
        cerr << "[FILE]: Failed to create / open file for reading.";
        exit(-100);
    }
}

void checkFileIsOpen(ifstream& file) {
    if (!file.is_open()) {
        cerr << "[FILE]: Failed to create / open file for reading.";
        exit(-100);
    }
}

void RLEFile() {
    string Original = "";
    string codedFpV = "";
    string codedGamma = "";
    string codedOmega = "";

    for (int i = 0; i < 8192; i++) Original += (rand() % 100 > 90 ? '1' : '0');

    cout << Original << endl << endl;

    ofstream codedOut;

    codedOut.open("Original.dat", ios::out | ios::binary);
    checkFileIsOpen(codedOut);
    writeBytesToFile(codedOut, Original);
    codedOut.close();

    codedOut.open("FpV.dat", ios::out | ios::binary);
    checkFileIsOpen(codedOut);
    writeBytesToFile(codedOut, RLE(Original, Coding::FpV));
    codedOut.close();

    codedOut.open("Gamma.dat", ios::out | ios::binary);
    checkFileIsOpen(codedOut);
    writeBytesToFile(codedOut, RLE(Original, Coding::Gamma));
    codedOut.close();

    codedOut.open("Omega.dat", ios::out | ios::binary);
    checkFileIsOpen(codedOut);
    writeBytesToFile(codedOut, RLE(Original, Coding::Omega));
    codedOut.close();

    ifstream codedIn;

    codedIn.open("Original.dat", ios::in | ios::binary);
    readBytesFromFile(codedIn, Original);
    codedIn.close();

    codedIn.open("FpV.dat", ios::in | ios::binary);
    readBytesFromFile(codedIn, codedFpV);
    codedIn.close();

    codedIn.open("Gamma.dat", ios::in | ios::binary);
    readBytesFromFile(codedIn, codedGamma);
    codedIn.close();

    codedIn.open("Omega.dat", ios::in | ios::binary);
    readBytesFromFile(codedIn, codedOmega);
    codedIn.close();

    cout << setw(18) << "Fixed + Variable: " << codedFpV.size() / (double)Original.size() * 100 << "%" << endl;
    cout << setw(18) << "Gamma: " << codedGamma.size() / (double)Original.size() * 100 << "%" << endl;
    cout << setw(18) << "Omega: " << codedOmega.size() / (double)Original.size() * 100 << "%" << endl;
}

size_t search(vector<pair<char, double>>& pair, const char& el) {
    for (size_t i = 0; i < pair.size(); i++) if (pair[i].first == el) return i;
    return string::npos;
}

bool alphabetComp(pair<char, double> a, pair<char, double> b) {
    return a.second > b.second;
}

void ShennonTable(const vector<int>& L, const vector<pair<char, double>>& alphabet, const vector<double>& Q, const vector<string>& keyword) {
    cout << endl;

    cout << "Shennon table" << endl << endl;

    int numberWidth = 0;
    int tmp = L.back();
    for (numberWidth = 1; (tmp /= 10); numberWidth++);

    stringstream tablecout;
    tablecout.setf(ios::fixed);
    tablecout << setprecision(DOUBLE_PRECISION);

    // Таблица
    tablecout << setw(1) << "a" << " | " << setw(DOUBLE_PRECISION + 2) << "Pi" + string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " << setw(DOUBLE_PRECISION + 2) << "Qi" + string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " << setw(numberWidth) << "L" << " | " << setw(keyword.back().size()) << "Keyword" << endl;
    tablecout << string(72, '—') << endl;
    for (size_t i = 0; i < alphabet.size(); i++) {
        tablecout << setw(1) << alphabet[i].first << " | " << setw(DOUBLE_PRECISION + 2) << alphabet[i].second << " | " << setw(DOUBLE_PRECISION) << Q[i] << " | " << setw(numberWidth) << L[i] << " | " << setw(keyword.back().size()) << keyword[i] << endl;
    }

    cout << tablecout.str();
}

tuple<map<char, string>, map<string, char>> Shennon(string text) {
    // Тестовый пример, все мы помним где в ASCII "Ё", поэтому
    // с моей домашкой не совпадает
    //text = "ЛЁВКИНИГОРЬАНДРЕЕВИЧ";

    // Создаём вектор пар для букв алфавита и их вероятностей
    vector<pair<char, double>> alphabet;

    // Заполняем уникальными буквами и частотой встречи её в тексте
    for (size_t i = 0; i < text.size(); i++) {
        size_t index = search(alphabet, text[i]);

        if (index != string::npos) alphabet[index].second += 1.0;
        else alphabet.push_back(make_pair(text[i], 1.0));
    }

    // Сортируем по не возрастанию алфавит
    sort(alphabet.begin(), alphabet.end(), alphabetComp);

    // Расчитываем вероятности
    for (size_t i = 0; i < alphabet.size(); i++) alphabet[i].second /= text.size();

    // Создаём вектор для комулятивных вероятностей
    vector<double> Q(alphabet.size()+1, 0);

    // Расчитываем комулятивные вероятности
    for (size_t i = 1; i < Q.size(); i++) Q[i] = (alphabet[i-(size_t)1].second + Q[i-(size_t)1]);

    // Создаём вектор для комулятивных вероятностей,
    // содержащих дробную часть комулятивных вероятностей в двоичном виде
    vector<string> Qbinary(alphabet.size(), "");

    // Заполняем переводя дробную часть комулятивных вероятностей
    // в двоичный вид
    for (size_t i = 0; i < Qbinary.size(); i++) Qbinary[i] = get<1>(ftobs(Q[i]));
    
    // Создаём вектор длин кодовых слов
    vector<int> L(alphabet.size(), 0);

    // Вычисляем длинны кодовых слов по формуле L[i] = ceil(-log2(alphabet[i]))
    for (size_t i = 0; i < alphabet.size(); i++) L[i] = static_cast<int>(ceil(-log2(alphabet[i].second)));

    // Создаём вектор кодовых слов
    vector<string> keyword(alphabet.size(), "");

    // Вычисляем кодовые слова как L[i] количество символов в векторе двоичных комулятивных
    // вероятностей с начала строки
    for (size_t i = 0; i < alphabet.size(); i++) keyword[i] = Qbinary[i].substr(0, L[i]);
   
    ShennonTable(L, alphabet, Q, keyword);

    // Создаём словарь map для хранения пар 'Символ' : "Кодовое слово"
    map<char, string> ChKeyword;
    map<string, char> KeywordCh;

    // Заполняем символами из алфавита и кодовыми словами
    for (size_t i = 0; i < alphabet.size(); i++) {
        ChKeyword.insert(make_pair(alphabet[i].first, keyword[i]));
        KeywordCh.insert(make_pair(keyword[i], alphabet[i].first));
    }

    if (KeywordCh.size() != keyword.size()) {
        cerr << endl << "[KEYWORD]: Keyword's are not unic!" << endl << endl;
        exit(15);
    }

    return make_tuple(ChKeyword, KeywordCh);
}

string codeByMap(map<char, string>& codeMap, string text) {
    string out = "";
    for (const auto& ch : text)
        out += codeMap.at(ch);
    return out;
}

string decodedByMap(map<string, char>& decodeMap, string text) {
    string out = "";
    string tmp = "";
    map<string, char>::iterator found = decodeMap.end();
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

streamsize getFileSize(ifstream& file) {
    file.ignore(numeric_limits<streamsize>::max());
    streamsize length = file.gcount();
    file.clear();
    file.seekg(0, ios_base::beg);
    return length;
}

void SFGm() {
    // Ставим русскую локаль
    setlocale(LC_ALL, "RUSSIAN");

    // Создаём строку для входного текста
    string text = "";
    string tmp;

    streamsize englishTextLenght = 0;

    ifstream englishText;
    englishText.open("EnglishANSI.txt", ios::in);
    checkFileIsOpen(englishText);
    englishTextLenght = getFileSize(englishText);
    while (getline(englishText, tmp)) text += tmp;
    englishText.close();

    cout << "Original text: " << endl << endl;
    cout << text << endl;

    tuple<map<char, string>, map<string, char>> ShennonCodeDecode = Shennon(text);

    map<char, string> ShennonCode = get<0>(ShennonCodeDecode);
    map<string, char> ShennonDecode = get<1>(ShennonCodeDecode);

    ofstream englishCoded;
    englishCoded.open("EnglishCodedANSI.txt", ios::out | ios::binary);
    checkFileIsOpen(englishCoded);
    writeBytesToFile(englishCoded, codeByMap(ShennonCode, text));
    englishCoded.close();

    string codedText = "";

    streamsize englishToDecodeLenght = 0;

    ifstream englishToDecode;
    englishToDecode.open("EnglishCodedANSI.txt", ios::in | ios::binary);
    checkFileIsOpen(englishToDecode);
    englishToDecodeLenght = getFileSize(englishToDecode);
    readBytesFromFile(englishToDecode, codedText);
    englishToDecode.close();

    cout << endl << endl;

    cout << "Coded text: " << endl << endl;
    cout << codedText;

    cout << endl << endl;

    string decodedText = decodedByMap(ShennonDecode, codedText);

    cout << "Decoded text: " << endl << endl;
    cout << decodedText;

    cout << endl << endl;

    cout << "Are original and transformed (encoded->decoded) text equal? Answer is: " << (decodedText == text ? "True" : "False") << endl << endl;

    stringstream fixedcout;
    fixedcout << setprecision(2) << fixed << englishToDecodeLenght / (double)englishTextLenght * 100;

    cout << "The file compression percentage is: " << fixedcout.str() << "%" << endl;
}

int main() {
    srand((unsigned int)time(NULL));

    //CodingTable();

    //RLEFile();

    SFGm();

    //system("pause");

    return 0;
}