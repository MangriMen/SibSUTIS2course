#include "BinaryString.h"
#include <sstream>
#include "const.h"

std::string BinaryString::deleteSpaces(std::string& s) {
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    return s;
}

std::tuple<std::string, int> BinaryString::itobs(int number) {
    std::string binaryNumber = "";
    int powerOfTwo = 0;

    if (number) {
        do {
            binaryNumber += std::to_string(number % 2);
            powerOfTwo++;
        } while (number /= 2);
        reverse(binaryNumber.begin(), binaryNumber.end());
    }

    return std::make_tuple(binaryNumber, powerOfTwo);
}

std::tuple<std::string, std::string> BinaryString::ftobs(double number) {
    int intPart = (int)(number);
    std::string intPartS = std::get<0>(itobs(intPart));
    double floatPart = number - intPart;
    std::string floatPartS = "";
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

std::vector<std::string>& BinaryString::split(const std::string& s, char delim, std::vector<std::string>& elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}