#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <iomanip>
#include <map>

#define NOT_FOUND string::npos
using namespace std;

constexpr int BLOCK = 64;

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

string Arithmetic(vector<pair<char, long double>> textAlphabet, const string& text) {
    vector<long double> r(text.size() + 1, 0);
    vector<pair<long double, long double>> border(text.size() + 1);
    vector<long double> Q(textAlphabet.size() + 1, 0);

    r[0] = 1;
    border[0].second = 1;

    for (int i = 0; i < (int)textAlphabet.size(); i++) {
        Q[i + 1] = textAlphabet[i].second + Q[i];
    }

    int m = 0;
    int i = 0;
    for (int i = 1; i <= (int)text.size(); i++) {
        int m = letterSearch(textAlphabet, text[i - 1]) + 1;
        border[i].first = border[i - 1].first + r[i - 1] * Q[m - 1];
        border[i].second = border[i - 1].first + r[i - 1] * Q[m];
        r[i] = border[i].second - border[i].first;
    }

    int count = (int)ceil(-log2(r.back()));
    string temp = floatToBinaryStr((border.back().second + border.back().first) / 2).second;
    
    if (count > (int)text.size()) return temp.substr(0, text.size());
    return temp.substr(0, count);
}

int main() {
    system("CHCP 1251");
    system("CLS");

    string text = "";
    string getlineBuffer = "";
    ifstream openedText;
    vector<pair<char, long double>> textAlphabet;

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

    long double ver = 0;
    // Расчитываем вероятности
    for (size_t i = 0; i < textAlphabet.size(); i++) {
        textAlphabet[i].second /= (long double)text.size();
        cout << textAlphabet[i].first << " " << textAlphabet[i].second << endl;
        ver += textAlphabet[i].second;
    }
    cout << endl << "Sum of probability: " << ver << endl << endl;

    int size = (int)ceil(text.size() / (double)BLOCK);
    cout << size << " blocks:" << endl << endl;

    for (int i = 0; i < (int)text.size(); i += BLOCK) {
        cout << Arithmetic(textAlphabet, text.substr(i, BLOCK));
        cout << endl;
    }
}
