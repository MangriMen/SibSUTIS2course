#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

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

void wCodeBody(string& word, int number) {
    if (number == 1) return;
    pair<string, int> tempPair = stringToBinary(number);

    word.insert(0, tempPair.first + " ");
    number = tempPair.first.length() - 1;

    wCodeBody(word, number);
}

string wCodeStart(int number) {
    if (!number) return "nan";

    string keyword = "0";
    wCodeBody(keyword, number);

    return keyword;
}

void createTab(size_t size) {
    cout << "Number\t| Fixed + Variable | y-code Alias | w-code Alias" << endl;
    for (int i = 0; i < (int)size; ++i)
        cout << setw(2) << i << "\t| " << setw(16) << fixedVariable(i) 
               << " | " << setw(12) << yCode(i) << " | " << setw(12) << wCodeStart(i) << endl;
}

vector<string>& split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) elems.push_back(item);
    return elems;
}

string deleteSpaces(const string& s) {
    string nospace;
    for (int i = 0; i < (int)s.size(); i++)
        if (s[i] != ' ')
            nospace += s[i];
    return nospace;
}

string encodeTyped(string number, int type) {
    vector<string> tmpSeries;
    string result = "";

    split(number, '1', tmpSeries);

    for (auto ser : tmpSeries)
        switch (type) {
        case 1:
            result += fixedVariable(ser.length() + 1);
            break;
        case 2:
            result += yCode(ser.length() + 1);
            break;
        case 3:
            result += wCodeStart(ser.length() + 1);
            break;
        default:
            return "";
        }

    result = deleteSpaces(result);

    return result;
}

int main() {
    int lastTabNumber = 0;
    string toCode = "";
    string Original = "";
    string encodedFPV = "";
    string encodedGamma = "";
    string encodedOmega = "";
    ofstream codedOut;

    cout << "Create a table up to number: ";
    cin >> lastTabNumber;

    createTab(++lastTabNumber);

    for (int i = 0; i < 4096; i++)
        toCode += (rand() % 100 > 85 ? '1' : '0');

    cout << endl << "Binary representation of encoded string:"
         << endl << endl << toCode;

    codedOut.open("original.dat", ios::binary);
    codedOut << toCode;
    codedOut.close();
    codedOut.open("FPV-encoded.dat", ios::binary);
    codedOut << encodeTyped(toCode, 1);
    codedOut.close();
    codedOut.open("y-encoded.dat", ios::binary);
    codedOut << encodeTyped(toCode, 2);
    codedOut.close();
    codedOut.open("w-encoded.dat", ios::binary);
    codedOut << encodeTyped(toCode, 3);
    codedOut.close();

    ifstream codedIn;
    codedIn.open("original.dat");
    getline(codedIn, Original);
    codedIn.close();
    codedIn.open("FPV-encoded.dat");
    getline(codedIn, encodedFPV);
    codedIn.close();
    codedIn.open("y-encoded.dat");
    getline(codedIn, encodedGamma);
    codedIn.close();
    codedIn.open("w-encoded.dat");
    getline(codedIn, encodedOmega);
    codedIn.close();

    cout << endl << endl;
    cout << "Fixed + Variable: " << encodedFPV.size() / (double)Original.size() * 100 << "%" << endl
         << "y-Code: " << encodedGamma.size() / (double)Original.size() * 100 << "%" << endl
         << "w-Code: " << encodedOmega.size() / (double)Original.size() * 100 << "%" << endl;

    return 0;
}