﻿#include <iostream>
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
#include "const.h"
#include "Lab11.h"
#include "BinaryString.h"

using namespace std;

enum class CodingT
{
    FpV,
    Gamma,
    Omega
};

string fixedVariable(int number) {
    tuple<string, int> convertedNum = BinaryString::itobs(number);

    string binaryNumber = get<0>(convertedNum);
    int power = get<1>(convertedNum);

    string keywordFirst = get<0>(BinaryString::itobs(power));
    string keywordSecond = "";

    if (power) keywordSecond = binaryNumber.substr(binaryNumber.length() - (static_cast<unsigned long long>(power) - 1), static_cast<unsigned long long>(power) - 1);

    return string(4 - keywordFirst.length(), '0') + keywordFirst + " " + keywordSecond;
}

string gammaCode(int number) {
    if (number) {
        tuple<string, int> convertedNum = BinaryString::itobs(number);
        string keywordFirst = string(static_cast<unsigned long long>(get<1>(convertedNum)) - 1, '0');
        string keywordSecond = get<0>(convertedNum);
        return keywordFirst + " " + keywordSecond;
    }
    
    return "";
}

void omegaCodeRec(string& word, int number) {
    if (number == 1) return;
    tuple<string, int> convertedNum = BinaryString::itobs(number);

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

string RLE(string number, CodingT type) {
    vector<string> tmpSeries;
    string outRLE = "";

    BinaryString::split(number, '1', tmpSeries);

    for (const string& ser : tmpSeries)
        switch (type)
        {
        case CodingT::FpV:
            outRLE += fixedVariable(static_cast<int>(ser.length()));
            break;
        case CodingT::Gamma:
            outRLE += gammaCode(static_cast<int>(ser.length()) + 1);
            break;
        case CodingT::Omega:
            outRLE += omegaCode(static_cast<int>(ser.length()) + 1);
            break;
        default:
            cerr << endl << "[RLE]: Unknown coding type!" << endl;
            exit(-10);
            break;
        }

    return BinaryString::deleteSpaces(outRLE);
}

string RLE(int number, CodingT type) {
    return RLE(get<0>(BinaryString::itobs(number)), type);
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
            setw(16) << RLE(i, CodingT::Gamma) <<
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
    writeBytesToFile(codedOut, RLE(Original, CodingT::FpV));
    codedOut.close();

    codedOut.open("Gamma.dat", ios::out | ios::binary);
    checkFileIsOpen(codedOut);
    writeBytesToFile(codedOut, RLE(Original, CodingT::Gamma));
    codedOut.close();

    codedOut.open("Omega.dat", ios::out | ios::binary);
    checkFileIsOpen(codedOut);
    writeBytesToFile(codedOut, RLE(Original, CodingT::Omega));
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

streamsize getFileSize(ifstream& file) {
    file.ignore(numeric_limits<streamsize>::max());
    streamsize length = file.gcount();
    file.clear();
    file.seekg(0, ios_base::beg);
    return length;
}

void SFGm() {

    // Строка для входного текста
    string text = "";
    string getlineBuffer;
    
    // Строка для закодированного текста
    string codedText = "";




    ifstream englishText;

    englishText.open("Lab11/EnglishANSI.txt", ios::in);
    checkFileIsOpen(englishText);
    streamsize englishTextLenght = getFileSize(englishText);
    while (getline(englishText, getlineBuffer)) text += getlineBuffer;
    englishText.close();
    getlineBuffer.clear();



    Lab11 coding(text);



    ofstream englishCoded;

    englishCoded.open("Lab11/EnglishCodedShennon.txt", ios::out | ios::binary);
    checkFileIsOpen(englishCoded);
    writeBytesToFile(englishCoded, coding.CodeBy(Lab11::Code::Shennon, text));
    englishCoded.close();


    englishCoded.open("Lab11/EnglishCodedHuffman.txt", ios::out | ios::binary);
    checkFileIsOpen(englishCoded);
    writeBytesToFile(englishCoded, coding.CodeBy(Lab11::Code::Huffman, text));
    englishCoded.close();


    englishCoded.open("Lab11/EnglishCodedGilbertMoore.txt", ios::out | ios::binary);
    checkFileIsOpen(englishCoded);
    writeBytesToFile(englishCoded, coding.CodeBy(Lab11::Code::GilbertMoore, text));
    englishCoded.close();




    ifstream englishToDecode;

    englishToDecode.open("Lab11/EnglishCodedShennon.txt", ios::in | ios::binary);
    checkFileIsOpen(englishToDecode);
    streamsize ShennonLenght = getFileSize(englishToDecode);
    readBytesFromFile(englishToDecode, codedText);
    string decodedShennon = coding.DecodeBy(Lab11::Code::Shennon, codedText);
    englishToDecode.close();


    englishToDecode.open("Lab11/EnglishCodedHuffman.txt", ios::in | ios::binary);
    checkFileIsOpen(englishToDecode);
    streamsize HuffmanLenght = getFileSize(englishToDecode);
    readBytesFromFile(englishToDecode, codedText);
    string decodedHuffman = coding.DecodeBy(Lab11::Code::Huffman, codedText);
    englishToDecode.close();

    
    englishToDecode.open("Lab11/EnglishCodedGilbertMoore.txt", ios::in | ios::binary);
    checkFileIsOpen(englishToDecode);
    streamsize GilbertMooreLenght = getFileSize(englishToDecode);
    readBytesFromFile(englishToDecode, codedText);
    string decodedGilbertMoore = coding.DecodeBy(Lab11::Code::GilbertMoore, codedText);
    englishToDecode.close();




    //cout << "Coded text(Shennon): " << endl << endl << codedText << endl << endl;
    //cout << "Coded text(Huffman): " << endl << endl << codedText << endl << endl << endl;
    //cout << "Coded text(GilbertMoore): " << endl << endl << codedText << endl << endl;


    //cout << "Original text: " << endl << endl << text << endl << endl;
    //cout << "Decoded text(Shennon): " << endl << endl << decodedShennon << endl << endl;
    //cout << "Decoded text(Huffman): " << endl << endl << decodedHuffman << endl << endl;
    //cout << "Decoded text(GilbertMoore): " << endl << endl << decodedGilbertMoore << endl << endl;




    stringstream fixedcout;

    cout << "Are original and transformed (encoded->decoded) text equal?" << endl << endl;

    cout << string(74, '-') << endl;
    cout <<
        setw(12) << "Method  " << " | " <<
        setw(12) << "Shennon  " << setw(3) << " | " <<
        setw(12) << "Fano    " << setw(3) << " | " <<
        setw(12) << "Huffman  " << setw(3) << " | " <<
        setw(12) << "GilbertMoore" << setw(3) << " | " << endl;
    cout << string(74, '-') << endl;
    cout <<
        setw(12) << "Is equal ?" << " | " <<
        setw(12) << (decodedShennon == text ? "True" : "False") + string(4, ' ') << setw(3) << " | " <<
        setw(12) << "False" + string(4, ' ') << setw(3) << " | " <<
        setw(12) << (decodedHuffman == text ? "True" : "False") + string(4, ' ') << setw(3) << " | " <<
        setw(12) << (decodedGilbertMoore == text ? "True" : "False") + string(4, ' ') << setw(3) << " | " << endl;
    cout << string(74, '-') << endl;
    fixedcout << setprecision(2) << fixed;
    fixedcout <<
        setw(12) << "Compression" << " | " <<
        setw(12) << to_string(ShennonLenght / (double)englishTextLenght * 100) + '%' + ' ' << setw(3) << " | " <<
        setw(12) << to_string(100) + '%' + ' ' << setw(3) << " | " <<
        setw(12) << to_string(HuffmanLenght / (double)englishTextLenght * 100) + '%' + ' ' << setw(3) << " | " <<
        setw(12) << to_string(GilbertMooreLenght / (double)englishTextLenght * 100) + '%' + ' ' << setw(3) << " | " << endl;
    cout << fixedcout.str();
    fixedcout.str("");
    cout << string(74, '-') << endl;
    




    cout << endl << endl;
    cout << string(69, '-') << endl;
    cout << setw(22) << "Alphabet entropy  " << " | " << setw(42) << "Average keyword lenght          " << " | " << endl;
    cout <<setw(24) << " |" << string(44, '-') << "| " << endl;
    cout <<
        setw(25) << " | " <<
        setw(12) << "Shennon   " << " | " <<
        setw(12) << "Huffman   " << " | " <<
        setw(12) << "GilbertMoore" << " | " << endl;
    cout << string(69, '-') << endl;
    cout <<
        setw(22) << coding.GetAlphabetEntropy()<< " | " <<
        setw(9) << coding.GetAverageKeywordLenght(Coding::Code::Shennon) << string(3, ' ') << " | " <<
        setw(9) << coding.GetAverageKeywordLenght(Coding::Code::Huffman) << string(3, ' ') << " | " <<
        setw(9) << coding.GetAverageKeywordLenght(Coding::Code::GilbertMoore) << string(3, ' ') << " | " << endl;
    cout << string(69, '-') << endl;

}

int main() {
    srand((unsigned int)time(NULL));
    system("chcp 1251");
    //CodingTable();

    //RLEFile();

    SFGm();

    //system("pause");

    return 0;
}