#include <iostream>
#include <string>
#include <tuple>
#include <iomanip>

using namespace std;

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

void createTab(int size) {
    cout << setw(6) << "Number" << "\t| Fixed + Variable | Alias gamma-code | Alias omega-code" << endl;
    for (int i = 0; i < size; ++i) {
        cout << setw(6) << i << "\t| " << setw(16) << fixedVariable(i) << " | " << setw(16) << gammaCode(i) << " | " << setw(16) << omegaCode(i) << endl;
    }
}

int main() {
    int rows = 0;
    cout << "Enter the number of rows: ";
    cin >> rows;

    createTab(rows);

    return 0;
}