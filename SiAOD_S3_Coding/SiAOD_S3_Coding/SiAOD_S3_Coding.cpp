#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
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
#include <cstdlib>
#include <experimental/filesystem>
#include "Employee2.h"
#include "const.h"
#include "Lab11.h"
#include "BinaryString.h"
#include <set>
#include "Employee4.h"

using namespace std;
namespace fs = std::experimental::filesystem;

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
            "\t| " << setw(16) << fixedVariable(i) << " | " <<
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
    bitset<CHAR_BIT> byte;
    size_t i = 0, b = 0;
    uint_fast32_t bitsToRead = static_cast<uint_fast32_t>(bitSequence.size());
    file.write((char*)&bitsToRead, sizeof(uint_fast32_t));
    while (i < bitSequence.size()) {
        byte.reset();
        for (b = 0; (i < bitSequence.size()) && (b < CHAR_BIT); i++, b++) byte.set(CHAR_BIT - 1 - b, (bitSequence[i] == '1'));
        file << static_cast<uint_fast8_t>(byte.to_ulong());
    }
}

void readBytesFromFile(ifstream& file, string& bitSequence) {
    bitSequence.resize(0);
    uint_fast32_t bitsToRead = 0;
    file.read((char*)&bitsToRead, sizeof(uint_fast32_t));
    uint_fast32_t bytesToRead = static_cast<uint_fast32_t>(ceil(bitsToRead / (double)CHAR_BIT));
    for (uint_fast32_t bit = 0; bit < bytesToRead; bit++) bitSequence += bitset<CHAR_BIT>(file.get()).to_string();
    bitSequence.resize(bitSequence.size() - (bitSequence.size() - bitsToRead), '#');
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

void Coding(int argc, char* argv[]) {

    cout << "Start coding..." << endl;

    bool isTextOut = false;
    bool isCodedTextOutput = false;
    bool isDecodedTextOutput = false;
    bool isTables = false;

    //int codings = (Lab11::Code::Shennon | Lab11::Code::Fano | Lab11::Code::Huffman | Lab11::Code::GilbertMoore);
    int codings = 0;

    set<string> args;
    args.insert("-Shennon");
    args.insert("-Huffman");
    args.insert("-Fano");
    args.insert("-GilbertMoore");

    args.insert("-t");
    args.insert("-c");
    args.insert("-d");
    args.insert("-table");


    string pathToFile = string(argv[1]);

    for (int i = 0; i < argc; i++) {
        if (args.find((string)argv[i]) != args.end()) {
            if ((string)argv[i] == "-t") {
                isTextOut = true;
                args.erase((string)argv[i]);
            }
            if ((string)argv[i] == "-c") {
                isCodedTextOutput = true;
                args.erase((string)argv[i]);
            }
            if ((string)argv[i] == "-d") {
                isDecodedTextOutput = true;
                args.erase((string)argv[i]);
            }
            if ((string)argv[i] == "-table") {
                isTables = true;
                args.erase((string)argv[i]);
            }
            if ((string)argv[i] == "-Shennon") {
                codings += Lab11::Code::Shennon;
                args.erase((string)argv[i]);
            }
            if ((string)argv[i] == "-Huffman") {
                codings += Lab11::Code::Huffman;
                args.erase((string)argv[i]);
            }
            if ((string)argv[i] == "-Fano") {
                codings += Lab11::Code::Fano;
                args.erase((string)argv[i]);
            }
            if ((string)argv[i] == "-GilbertMoore") {
                codings += Lab11::Code::GilbertMoore;
                args.erase((string)argv[i]);
            }
        }
    }

    // Строка для входного текста
    string text = "";
    string getlineBuffer = "";

    // Строка для закодированного текста
    string codedText = "";


    streamsize baseLenght = 0;

    string directory = "";

    unsigned char charText = unsigned char();

    ifstream fileIn;
    if (pathToFile.substr(pathToFile.size() - 3, 3) == "txt") {
        fileIn.open(pathToFile, ios::in);
        checkFileIsOpen(fileIn);
        baseLenght = getFileSize(fileIn);
        while (getline(fileIn, getlineBuffer)) text += getlineBuffer;
        fileIn.close();
        getlineBuffer.clear();
        directory = "English";
    }
    else if (pathToFile.substr(pathToFile.size() - 3, 3) == "dat")
    {
        fileIn.open(pathToFile, ios::in | ios::binary);
        checkFileIsOpen(fileIn);
        baseLenght = getFileSize(fileIn);
        while (fileIn.read((char*)&charText, sizeof(charText))) text += charText;
        fileIn.close();
        getlineBuffer.clear();
        directory = "CourseWork";
    }

    if (isTextOut) cout << text << endl << endl;


    Lab11 coding(text, codings);

    fs::create_directory(directory);

    ofstream fileOut;
    if (codings & Lab11::Code::Shennon) {
        fileOut.open(directory + "/Shennon.bin", ios::out | ios::binary);
        checkFileIsOpen(fileOut);
        writeBytesToFile(fileOut, coding.CodeBy(Lab11::Code::Shennon, text));
        fileOut.close();
    }
    if (codings & Lab11::Code::Fano) {
        fileOut.open(directory + "/Fano.bin", ios::out | ios::binary);
        checkFileIsOpen(fileOut);
        writeBytesToFile(fileOut, coding.CodeBy(Lab11::Code::Fano, text));
        fileOut.close();
    }
    if (codings & Lab11::Code::Huffman) {
        fileOut.open(directory + "/Huffman.bin", ios::out | ios::binary);
        checkFileIsOpen(fileOut);
        writeBytesToFile(fileOut, coding.CodeBy(Lab11::Code::Huffman, text));
        fileOut.close();
    }
    if (codings & Lab11::Code::GilbertMoore) {
        fileOut.open(directory + "/GilbertMoore.bin", ios::out | ios::binary);
        checkFileIsOpen(fileOut);
        writeBytesToFile(fileOut, coding.CodeBy(Lab11::Code::GilbertMoore, text));
        fileOut.close();
    }
    fileOut.close();


    ifstream englishToDecode;

    streamsize ShennonLenght = 0;
    streamsize FanoLenght = 0;
    streamsize HuffmanLenght = 0;
    streamsize GilbertMooreLenght = 0;

    string decodedShennon = "";
    string decodedFano = "";
    string decodedHuffman = "";
    string decodedGilbertMoore = "";

    if (codings & Lab11::Code::Shennon) {
        englishToDecode.open(directory + "/Shennon.bin", ios::in | ios::binary);
        checkFileIsOpen(englishToDecode);
        ShennonLenght = getFileSize(englishToDecode);
        readBytesFromFile(englishToDecode, codedText);
        if (isCodedTextOutput) cout << "Coded text(Shennon): " << endl << endl << codedText << endl << endl;
        decodedShennon = coding.DecodeBy(Lab11::Code::Shennon, codedText);
        englishToDecode.close();
    }
    if (codings & Lab11::Code::Fano) {
        englishToDecode.open(directory + "/Fano.bin", ios::in | ios::binary);
        checkFileIsOpen(englishToDecode);
        FanoLenght = getFileSize(englishToDecode);
        readBytesFromFile(englishToDecode, codedText);
        if (isCodedTextOutput) cout << "Coded text(Fano): " << endl << endl << codedText << endl << endl;
        decodedFano = coding.DecodeBy(Lab11::Code::Fano, codedText);
        englishToDecode.close();
    }
    if (codings & Lab11::Code::Huffman) {
        englishToDecode.open(directory + "/Huffman.bin", ios::in | ios::binary);
        checkFileIsOpen(englishToDecode);
        HuffmanLenght = getFileSize(englishToDecode);
        readBytesFromFile(englishToDecode, codedText);
        if (isCodedTextOutput) cout << "Coded text(Huffman): " << endl << endl << codedText << endl << endl;
        decodedHuffman = coding.DecodeBy(Lab11::Code::Huffman, codedText);
        englishToDecode.close();
    }
    if (codings & Lab11::Code::GilbertMoore) {
        englishToDecode.open(directory + "/GilbertMoore.bin", ios::in | ios::binary);
        checkFileIsOpen(englishToDecode);
        GilbertMooreLenght = getFileSize(englishToDecode);
        readBytesFromFile(englishToDecode, codedText);
        if (isCodedTextOutput) cout << "Coded text(GilbertMoore): " << endl << endl << codedText << endl << endl;
        decodedGilbertMoore = coding.DecodeBy(Lab11::Code::GilbertMoore, codedText);
        englishToDecode.close();
    }



    if (isDecodedTextOutput) {
        if (codings & Lab11::Code::Shennon) {
            cout << "Decoded text(Shennon): " << endl << endl << decodedShennon << endl << endl;
        }
        if (codings & Lab11::Code::Fano) {
            cout << "Decoded text(Fano): " << endl << endl << decodedFano << endl << endl;
        }
        if (codings & Lab11::Code::Huffman) {
            cout << "Decoded text(Huffman): " << endl << endl << decodedHuffman << endl << endl;
        }
        if (codings & Lab11::Code::GilbertMoore) {
            cout << "Decoded text(GilbertMoore): " << endl << endl << decodedGilbertMoore << endl << endl;
        }
    }


    if (isTables) {
        cout << "Are original and transformed (encoded->decoded) text equal?" << endl << endl;

        cout << string(74, '-') << endl;
        cout << setw(12) << "Method  " << " | ";

        if (codings & Lab11::Code::Shennon) {
            cout << setw(12) << "Shennon  " << setw(3) << " | ";
        }
        if (codings & Lab11::Code::Fano) {
            cout << setw(12) << "Fano    " << setw(3) << " | ";
        }
        if (codings & Lab11::Code::Huffman) {
            cout << setw(12) << "Huffman  " << setw(3) << " | ";
        }
        if (codings & Lab11::Code::GilbertMoore) {
            cout << setw(12) << "GilbertMoore" << setw(3) << " | ";
        }

        cout << endl;
        cout << string(74, '-') << endl;
        cout << setw(12) << "Is equal ?" << " | ";

        if (codings & Lab11::Code::Shennon) {
            cout << setw(12) << (decodedShennon == text ? "True" : "False") + string(4, ' ') << setw(3) << " | ";
        }
        if (codings & Lab11::Code::Fano) {
            cout << setw(12) << (decodedFano == text ? "True" : "False") + string(4, ' ') << setw(3) << " | ";
        }
        if (codings & Lab11::Code::Huffman) {
            cout << setw(12) << (decodedHuffman == text ? "True" : "False") + string(4, ' ') << setw(3) << " | ";
        }
        if (codings & Lab11::Code::GilbertMoore) {
            cout << setw(12) << (decodedGilbertMoore == text ? "True" : "False") + string(4, ' ') << setw(3) << " | ";
        }

        cout << endl;
        cout << string(74, '-') << endl;

        stringstream fixedcout;

        fixedcout << setprecision(2) << fixed;
        fixedcout << setw(12) << "Compression" << " | ";

        if (codings & Lab11::Code::Shennon) {
            fixedcout << setw(12) << to_string(ShennonLenght / (double)baseLenght * 100) + '%' + ' ' << setw(3) << " | ";
        }
        if (codings & Lab11::Code::Fano) {
            fixedcout << setw(12) << to_string(FanoLenght / (double)baseLenght * 100) + '%' + ' ' << setw(3) << " | ";
        }
        if (codings & Lab11::Code::Huffman) {
            fixedcout << setw(12) << to_string(HuffmanLenght / (double)baseLenght * 100) + '%' + ' ' << setw(3) << " | ";
        }
        if (codings & Lab11::Code::GilbertMoore) {
            fixedcout << setw(12) << to_string(GilbertMooreLenght / (double)baseLenght * 100) + '%' + ' ' << setw(3) << " | ";
        }



        fixedcout << endl;
        cout << fixedcout.str();
        fixedcout.str("");
        cout << string(74, '-') << endl;

        cout << endl << endl;
        cout << string(84, '-') << endl;
        cout << setw(22) << "Alphabet entropy  " << " | " << setw(57) << "Average keyword lenght" + string(16, ' ') << " | " << endl;
        cout << setw(24) << " |" << string(59, '-') << "| " << endl;
        cout << setw(25) << " | ";

        if (codings & Lab11::Code::Shennon) {
            cout << setw(12) << "Shennon   " << " | ";
        }
        if (codings & Lab11::Code::Fano) {
            cout << setw(12) << "Fano    " << " | ";
        }
        if (codings & Lab11::Code::Huffman) {
            cout << setw(12) << "Huffman   " << " | ";
        }
        if (codings & Lab11::Code::GilbertMoore) {
            cout << setw(12) << "GilbertMoore" << " | ";
        }

        cout << endl;
        cout << string(84, '-') << endl;
        cout << setw(22) << coding.GetAlphabetEntropy() << " | ";

        if (codings & Lab11::Code::Shennon) {
            cout << setw(9) << coding.GetAverageKeywordLenght(Coding::Code::Shennon) << string(3, ' ') << " | ";
        }
        if (codings & Lab11::Code::Fano) {
            cout << setw(9) << coding.GetAverageKeywordLenght(Coding::Code::Fano) << string(3, ' ') << " | ";
        }
        if (codings & Lab11::Code::Huffman) {
            cout << setw(9) << coding.GetAverageKeywordLenght(Coding::Code::Huffman) << string(3, ' ') << " | ";
        }
        if (codings & Lab11::Code::GilbertMoore) {
            cout << setw(9) << coding.GetAverageKeywordLenght(Coding::Code::GilbertMoore) << string(3, ' ') << " | ";
        }
        cout << endl;
        cout << string(84, '-') << endl;
    }

    cout << endl << "Completed successfully" << endl;
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    //system("chcp 1251");
    //CodingTable();
    //RLEFile();
    //SFGm();

    Coding(argc, argv);

    system("pause");

    return 0;
}