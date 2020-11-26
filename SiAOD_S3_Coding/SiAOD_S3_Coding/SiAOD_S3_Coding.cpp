#include <iostream>
#include <ctime>
#include <string>
#include <tuple>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

enum class Coding
{
    FpV,
    Gamma,
    Omega
};

string deleteSpaces(const string& s) {
    string nospace;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] != ' ')
            nospace += s[i];
    return nospace;
}

tuple<string, int> stob(int number) {
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

vector<string>& split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string fixedVariable(int number) {
    tuple<string, int> convertedNum = stob(number);

    string binaryNumber = get<0>(convertedNum);
    int power = get<1>(convertedNum);

    string keywordFirst = get<0>(stob(power));
    string keywordSecond = "";

    if (power) keywordSecond = binaryNumber.substr(binaryNumber.length() - (power - 1), power - 1);

    return string(4 - keywordFirst.length(), '0') + keywordFirst + " " + keywordSecond;
}

string gammaCode(int number) {
    if (number) {
        tuple<string, int> convertedNum = stob(number);
        string keywordFirst = string(get<1>(convertedNum) - 1, '0');
        string keywordSecond = get<0>(convertedNum);
        return keywordFirst + " " + keywordSecond;
    }
    
    return "";
}

void omegaCodeRec(string& word, int number) {
    if (number == 1) return;
    tuple<string, int> convertedNum = stob(number);

    word.insert(0, get<0>(convertedNum) + " ");
    number = get<0>(convertedNum).length() - 1;

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
            outRLE += fixedVariable(ser.length());
            break;
        case Coding::Gamma:
            outRLE += gammaCode(ser.length() + 1);
            break;
        case Coding::Omega:
            outRLE += omegaCode(ser.length() + 1);
            break;
        default:
            cerr << endl << "[RLE]: Unknown coding type!" << endl;
            exit(-10);
            break;
        }

    return deleteSpaces(outRLE);
}

string RLE(int number, Coding type) {
    return RLE(get<0>(stob(number)), type);
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

void RLEFile() {
    string toCode = "";

    for (int i = 0; i < 2048; i++)
        toCode += (rand() % 100 > 85 ? '1' : '0');

    cout << toCode << endl;
    cout << endl;

    ofstream codedOut;

    codedOut.open("Original.dat", ios::binary);
    codedOut << toCode;
    codedOut.close();

    codedOut.open("FpV.dat", ios::binary);
    codedOut << RLE(toCode, Coding::FpV);
    codedOut.close();

    codedOut.open("Gamma.dat", ios::binary);
    codedOut << RLE(toCode, Coding::Gamma);
    codedOut.close();

    codedOut.open("Omega.dat", ios::binary);
    codedOut << RLE(toCode, Coding::Omega);
    codedOut.close();

    string Original = "";
    string codedFpV = "";
    string codedGamma = "";
    string codedOmega = "";

    ifstream codedIn;

    codedIn.open("Original.dat");
    getline(codedIn, Original);
    codedIn.close();

    codedIn.open("FpV.dat");
    getline(codedIn, codedFpV);
    codedIn.close();

    codedIn.open("Gamma.dat");
    getline(codedIn, codedGamma);
    codedIn.close();

    codedIn.open("Omega.dat");
    getline(codedIn, codedOmega);
    codedIn.close();

    cout << setw(18) << "Fixed + Variable: " << codedFpV.size() / (double)Original.size() * 100 << "%" << endl;
    cout << setw(18) << "Gamma: " << codedGamma.size() / (double)Original.size() * 100 << "%" << endl;
    cout << setw(18) << "Omega: " << codedOmega.size() / (double)Original.size() * 100 << "%" << endl;
}

int main() {
    srand((unsigned int)time(NULL));

    //CodingTable();

    //RLEFile();

    return 0;
}