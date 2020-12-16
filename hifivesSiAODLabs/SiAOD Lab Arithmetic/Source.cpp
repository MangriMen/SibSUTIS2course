#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <iomanip>
#include <map>

#define NOT_FOUND string::npos
using namespace std;

int letterSearch(const vector<pair<char, long double>>& textAlphabet, const char& el) {
    for (int i = 0; i < textAlphabet.size(); i++)
        if (textAlphabet[i].first == el) return i;

    return NOT_FOUND;
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

pair<string, string> floatToBinaryStr(long double number) {
    int intPart = (int)(number);
    float floatPart = number - intPart;
    string beforePoint = intToBinaryStr(intPart).first;
    string afterPoint = "";

    for (int i = 0; i < 32; i++) {
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

string arithmetic(vector<pair<char, long double>> textAlphabet, const string& text) {
    vector<long double> Q(textAlphabet.size() + 1, 0);              // ������������ ����������� ������
    vector<long double> r(text.size() + 1, 1);                      // ����� ���������� �� ������ ������
    vector<pair<long double, long double>> border(text.size() + 1); // ������� ���������
    
    border[0].second = 1;

    for (int i = 0; i < textAlphabet.size(); i++) { // ��������� ������ ����������� ������������ 
        Q[i + 1] = textAlphabet[i].second + Q[i]; 
    }

    for (int i = 1; i <= text.size(); i++) {
        int m = letterSearch(textAlphabet, text[i - 1]) + 1;            // ������ �������� � �������
        border[i].first = border[i - 1].first + r[i - 1] * Q[m - 1];    // �� �������� ���
        border[i].second = border[i - 1].first + r[i - 1] * Q[m];       // �� �������� ���
        r[i] = border[i].second - border[i].first;                      //
    }

    int count = (int)ceil(-log2(r.back()));
    string temp = floatToBinaryStr((border.back().second + border.back().first) / 2).second;
    
    return temp.substr(0, count);
}

int main() {
    system("CHCP 1251");
    system("CLS");

    string text = "";
    string getlineBuffer = "";
    ifstream openedText;
    vector<pair<char, long double>> textAlphabet;
    const int blockSize = 32;

    openedText.open("textToWorkWith.txt", ios::in);
    while (getline(openedText, getlineBuffer)) text += getlineBuffer;
    openedText.close();

    for (int i = 0; i < text.size(); i++) {
        int letter = letterSearch(textAlphabet, text[i]);

        if (letter == NOT_FOUND) {
            textAlphabet.push_back(make_pair(text[i], 1.0));
        } else {
            textAlphabet[letter].second += 1.0;
        }
    }
    
    // ������� ��������
    long double probability = 0;
    for (int i = 0; i < textAlphabet.size(); i++) {
        textAlphabet[i].second /= (long double)text.size();
        cout << textAlphabet[i].first << " " << textAlphabet[i].second << endl;
        probability += textAlphabet[i].second;
    }
    //

    for (int i = 0; i < (int)text.size(); i += blockSize)
        cout << arithmetic(textAlphabet, text.substr(i, blockSize)) << endl;
    
    cout << endl << probability << " - ����� ������������." << endl << endl;
    cout << ceil(text.size() / (double)blockSize) << " - ���-�� ������" << endl << endl;
    
    return 0;
}
