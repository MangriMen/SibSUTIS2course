#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include "..\coursework\DatabaseNode.h"

#define NOT_FOUND string::npos
using namespace std;

namespace Comparator {
    bool charByValue(const pair<char, double>& a, const pair<char, double>& b) {
        return a.second > b.second;
    }

    bool charByKey(const pair<char, double>& a, const pair<char, double>& b) {
        return a.first <= b.first;
    }
}

pair<string, int> intToBinaryStr(int number) {
    string binaryNumber = "";
    int powerOfTwo = 0;

    if (number) {
        do {
            binaryNumber += to_string(number % 2);
            powerOfTwo++;
        } while (number /= 2);
        reverse(binaryNumber.begin(), binaryNumber.end());
    }

    return { binaryNumber, powerOfTwo };
}

pair<string, string> floatToBinaryStr(float number) {
    int intPart = (int)(number);
    float floatPart = number - intPart;
    string beforePoint = intToBinaryStr(intPart).first;
    string afterPoint = "";

    // 16 - number of simbols after point
    for (int i = 0; i < 16; i++) {
        floatPart = floatPart * 2;
        if (floatPart >= 1) {
            afterPoint += '1';
            floatPart -= 1;
        }
        else {
            afterPoint += '0';
        }
    }

    return { beforePoint, afterPoint };
}

void printTab(const vector<pair<char, double>>& alphabet, const map<char, string>& keywordM) {
    cout << endl << "A  " << "Pi\t\t" << "L\t" << "Keyword" << endl;
    cout << string(55, '=') << endl;
    for (int i = 0; i < alphabet.size(); i++) {
        cout << alphabet[i].first << "  " << alphabet[i].second
            << "\t" << keywordM.at(alphabet[i].first).size()
            << "\t" << keywordM.at(alphabet[i].first) << endl;
    }
    cout << string(55, '=') << endl << endl << endl;
}

int letterSearch(const vector<pair<char, double>>& textAlphabet, const char& el) {
    for (int i = 0; i < textAlphabet.size(); i++)
        if (textAlphabet[i].first == el) return i;

    return NOT_FOUND;
}

double alphabetEntropy(vector<pair<char, double>> textAlphabet) {
    double entropy = 0;

    for (int i = 0; i < textAlphabet.size(); i++)
        entropy += -textAlphabet[i].second * log2(textAlphabet[i].second);

    return entropy;
}

double averageKeywordLenght(const map<char, string>& keywordM, vector<pair<char, double>> textAlphabet) {
    double averageLenght = 0;

    for (int i = 0; i < textAlphabet.size(); i++)
        averageLenght += textAlphabet[i].second * keywordM.at(textAlphabet[i].first).size();

    return averageLenght;
}

map<char, string> createGilbertMooreBM(vector<pair<char, double>> alphabet) {
    vector<double> Q(alphabet.size(), 0);
    vector<string> Qbinary(alphabet.size(), "");
    vector<string> keyword(alphabet.size(), "");
    map<char, string> keywordM;

    sort(alphabet.begin(), alphabet.end(), Comparator::charByKey);

    for (int i = 0; i < Q.size(); i++) {
        for (int j = 0; j < i; j++)
            Q[i] += alphabet[j].second;
        Q[i] += alphabet[i].second * 0.5f;
        Qbinary[i] = floatToBinaryStr(Q[i]).second;
        keyword[i] = Qbinary[i].substr(0, (int)((ceil(-log2(alphabet[i].second))) + 1));
    }

    for (int i = 0; i < alphabet.size(); i++) keywordM.insert({ alphabet[i].first, keyword[i] });

    cout << string(55, '=') << endl;
    cout << "GilbertMoore Table" << endl;
    cout << string(55, '=');
    printTab(alphabet, keywordM);

    return keywordM;
}

int main() {
    ifstream dbFile("testBase2.dat", ios::in | ios::binary);
    if (!dbFile) {
        dbFile.clear();
        cout << "Error: database file was not found";
        return 1;
    }
    dbFile.ignore(numeric_limits<streamsize>::max());
    streamsize length = dbFile.gcount();
    dbFile.clear();
    dbFile.seekg(0, ios_base::beg);
    int nodesNumber = length / sizeof(DatabaseNode);

    string textToEncode = "";
    for (int i = 0; i < nodesNumber; ++i) {
        DatabaseNode tempNode;
        tempNode.nodeFill(dbFile);
        textToEncode += (string)tempNode.employeeFullName + to_string(tempNode.departmentNumber) + (string)tempNode.post + (string)tempNode.birthDate;
    }
    
    vector<pair<char, double>> textAlphabet;
    map<char, string> GilbertMooreM;

    for (int i = 0; i < textToEncode.length(); i++) {
        int letter = letterSearch(textAlphabet, textToEncode[i]);

        if (letter != NOT_FOUND) {
            textAlphabet[letter].second += 1.0;
        }
        else {
            textAlphabet.push_back(make_pair(textToEncode[i], 1.0));
        }
    }

    for (int i = 0; i < textAlphabet.size(); i++)
        textAlphabet[i].second /= textToEncode.length();

    sort(textAlphabet.begin(), textAlphabet.end(), Comparator::charByValue);
    GilbertMooreM = createGilbertMooreBM(textAlphabet);

    // Entrophy to code length
    cout << endl << endl;
    cout << string(55, '=') << endl;
    cout << "Alphabet entrophy:\t\t\t" << alphabetEntropy(textAlphabet) << endl;
    cout << string(55, '=') << endl;
    cout << "Gilbert-Moore average code length:\t" << averageKeywordLenght(GilbertMooreM, textAlphabet) << endl;
    cout << string(55, '=') << endl;

    dbFile.close();
}