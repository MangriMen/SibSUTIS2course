#include <iostream>
#include <string>
#include <iomanip>
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

int main() {
    int lastTabNumber = 0;
    cout << "Create a table up to number: ";
    cin >> lastTabNumber;

    createTab(++lastTabNumber);

    return 0;
}