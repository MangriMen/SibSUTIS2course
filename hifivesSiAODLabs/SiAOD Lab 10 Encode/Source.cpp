#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <fstream>
using namespace std;

enum class RLEType {
    FPV,
    Gamma,
    Omega
};

pair<string, int> stringToBinary(int number) {
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

string fixedVariable(int number) {
    string keywordFirst;
    string keywordSecond;
    pair<string, int> tempPair = stringToBinary(number);
    string binaryNumber = tempPair.first;
    int power = tempPair.second;

    keywordFirst = stringToBinary(power).first;
    if (power) keywordSecond = binaryNumber.substr(binaryNumber.length() - (power - 1), power - 1);

    return string(4 - keywordFirst.length(), '0') + keywordFirst + " " + keywordSecond;
}

string yCode(int number) {
    if (!number) return "nan";

    pair<string, int> tempPair = stringToBinary(number);
    string keywordFirst = string(tempPair.second - 1, '0');
    string keywordSecond = tempPair.first;

    return keywordFirst + " " + keywordSecond;
}

void wCodeRec(string& word, int number) {
    if (number == 1) return;
    pair<string, int> tempPair = stringToBinary(number);

    word.insert(0, tempPair.first + " ");
    number = tempPair.first.length() - 1;

    wCodeRec(word, number);
}

string wCode(int number) {
    if (!number) return "nan";

    string keyword = "0";
    wCodeRec(keyword, number);

    return keyword;
}

void createTab(size_t size) {
    cout << "Number\t| Fixed + Variable | y-code Alias | w-code Alias" << endl;
    for (int i = 0; i < size; ++i) {
        cout << setw(2) << i << "\t| " << setw(16) << fixedVariable(i) 
               << " | " << setw(12) << yCode(i) << " | " << setw(12) << wCode(i) << endl;
    }
}

vector<string>& split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string deleteSpaces(const string& s) {
    string nospace;
    for (int i = 0; i < s.size(); i++)
        if (s[i] != ' ')
            nospace += s[i];
    return nospace;
}

string RLE(int number, RLEType type) {
    pair<string, int> convertedNum = stringToBinary(number);
    vector<string> tmpSeries;
    string outRLE = "";

    split(convertedNum.first, '1', tmpSeries);

    for (auto ser : tmpSeries)
        switch (type)
        {
        case RLEType::FPV:
            outRLE += fixedVariable(ser.length() + 1);
            break;
        case RLEType::Gamma:
            outRLE += yCode(ser.length() + 1);
            break;
        case RLEType::Omega:
            outRLE += wCode(ser.length() + 1);
            break;
        default:
            cerr << endl << "[RLE]: Unknown coding type!" << endl;
            exit(-10);
            break;
        }

    outRLE = deleteSpaces(outRLE);

    return outRLE;
}

string RLE(string number, RLEType type) {
    vector<string> tmpSeries;
    string outRLE = "";

    split(number, '1', tmpSeries);

    for (auto ser : tmpSeries)
        switch (type) {
        case RLEType::FPV:
            outRLE += fixedVariable(ser.length() + 1);
            break;
        case RLEType::Gamma:
            outRLE += yCode(ser.length() + 1);
            break;
        case RLEType::Omega:
            outRLE += wCode(ser.length() + 1);
            break;
        default:
            cerr << endl << "[RLE]: Unknown coding type!" << endl;
            exit(-10);
            break;
        }

    outRLE = deleteSpaces(outRLE);

    return outRLE;
}

int main() {
    int lastTabNumber = 0;
    cout << "Create a table up to number: ";
    cin >> lastTabNumber;

    createTab(++lastTabNumber);

    string toCode = "";

    for (int i = 0; i < 1024; i++)
        toCode += (rand() % 100 > 85 ? '1' : '0');

    cout << toCode << endl;
    cout << endl;

    ofstream codedOut;

    codedOut.open("Original.dat", ios::binary);
    codedOut << toCode;
    codedOut.close();

    codedOut.open("FPV.dat", ios::binary);
    codedOut << RLE(toCode, RLEType::FPV);
    codedOut.close();

    codedOut.open("Gamma.dat", ios::binary);
    codedOut << RLE(toCode, RLEType::Gamma);
    codedOut.close();

    codedOut.open("Omega.dat", ios::binary);
    codedOut << RLE(toCode, RLEType::Omega);
    codedOut.close();

    string Original = "";
    string codedFPV = "";
    string codedGamma = "";
    string codedOmega = "";

    ifstream codedIn;

    codedIn.open("Original.dat");
    getline(codedIn, Original);
    codedIn.close();

    codedIn.open("FPV.dat");
    getline(codedIn, codedFPV);
    codedIn.close();

    codedIn.open("Gamma.dat");
    getline(codedIn, codedGamma);
    codedIn.close();

    codedIn.open("Omega.dat");
    getline(codedIn, codedOmega);
    codedIn.close();

    cout << setw(18) << "Fixed + Variable: " << codedFPV.size() / (double)Original.size() * 100 << "%" << endl;
    cout << setw(18) << "Gamma: " << codedGamma.size() / (double)Original.size() * 100 << "%" << endl;
    cout << setw(18) << "Omega: " << codedOmega.size() / (double)Original.size() * 100 << "%" << endl;

    return 0;
}