#include "Coding.h"
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>
#include <map>
#include <sstream>
#include "BinaryString.h"
#include "const.h"

bool Coding::alphabetCompValue(std::pair<char, double> a, std::pair<char, double> b) {
    return a.second > b.second;
}

bool Coding::alphabetCompKeyR(std::pair<char, double> a, std::pair<char, double> b) {
    return a.first < b.first;
}

void DisplayTable(const std::vector<std::pair<char, double>>& alphabet, const boost::bimap<char, std::string>& keywordBM, const std::vector<double>& Q, Coding::Code type) {
    std::cout << std::endl;

    switch (type)
    {
    case Coding::Code::Shennon:
        std::cout << "Shennon";
        break;
    case Coding::Code::Huffman:
        std::cout << "Huffman";
        break;
    case Coding::Code::Fano:
        std::cout << "Fano";
        break;
    case Coding::Code::GilbertMoore:
        std::cout << "GilbertMoore";
        break;
    default:
        break;
    }
    std::cout << " Table" << std::endl << std::endl;

    int maxKeywordLenght = 0;
    for (auto& el : keywordBM.left)
        if (static_cast<int>(el.second.size()) > maxKeywordLenght) maxKeywordLenght = static_cast<int>(el.second.size());

    int numberWidth = 0;
    int tmp = maxKeywordLenght;
    for (numberWidth = 1; (tmp /= 10); numberWidth++);

    std::stringstream tablecout;
    tablecout.setf(std::ios::fixed);
    tablecout << std::setprecision(DOUBLE_PRECISION);

    // �������
    tablecout <<
        std::setw(1) << "a" << " | " <<
        std::setw(DOUBLE_PRECISION + 2) << "Pi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " <<
        std::setw(DOUBLE_PRECISION + 2) << "Qi" + std::string(((DOUBLE_PRECISION + 2) >> 1) - 1, ' ') << " | " <<
        std::setw(numberWidth) << "L" << " | " <<
        std::setw(maxKeywordLenght) << "Keyword" << std::endl;

    tablecout << std::string((DOUBLE_PRECISION + 2) * 2 + numberWidth + maxKeywordLenght + (3 * 4) + 1, '-') << std::endl;

    for (size_t i = 0; i < alphabet.size(); i++) {
        tablecout <<
            std::setw(1) << alphabet[i].first << " | " <<
            std::setw(DOUBLE_PRECISION + 2) << alphabet[i].second << " | " <<
            std::setw(DOUBLE_PRECISION) << Q[i + (type == Coding::Code::GilbertMoore)] << " | " <<
            std::setw(numberWidth) << keywordBM.left.at(alphabet[i].first).size() << " | " <<
            std::setw(maxKeywordLenght) << keywordBM.left.at(alphabet[i].first) << std::endl;
    }

    std::cout << tablecout.str() << std::endl;
}

double* P = nullptr;

int* Len = nullptr;

int** CH = new int* [100];

int Up(int n, double q) {
    int j = 0;
    for (int i = (n - 1); i > 2; i--) {
        if (P[i - 1] <= q) {
            P[i] = P[i - 1];
        }
        else {
            j = i;
        }
    }

    P[j] = q;
    return j;
}

void Down(int n, int j) {
    int S = *CH[j];

    int L = Len[j];
    for (int i = j; i < n - 2; j--) {
        CH[i] = CH[i + 1];
        Len[i] = Len[i + 1];
    }
    CH[n - 1] = &S;
    CH[n] = &S;
    CH[n - 1][L + 1] = 0;
    CH[n][L + 1] = 1;
    Len[n - 1] = L + 1;
    Len[n] = L + 1;

}

void buildHuffman(int n, double* P) {
    if (n == 2) {
        CH[1][1] = 0;
        Len[1] = 1;
        CH[2][1] = 1;
        Len[2] = 1;
    }
    else {
        double q = P[n - 1] + P[n];
        int j = Up(n, q);
        buildHuffman(n - 1, P);
        Down(n, j);
    }
}

std::vector<int> Lenght(100, 0);

std::vector<std::vector<int>> C(1000, std::vector<int>(1000, 0));

int getMedian(int L, int R) {
    //double sL = 0;
    //for (int i = 0; i < R - 1; i++) {
    //    sL = sL + P[i];
    //}
    //double sR = P[R];
    //int m = R;
    //while (sL >= sR) {
    //    m = m - 1;
    //    sL = sL - P[m];
    //    sR = sR + P[m];
    //}

    //return m;
    return 0;
}

void buildFano(int L, int R, int k) {
    //if (L < R) {
    //    k = k+1;
    //    int m = getMedian(L, R);
    //    for (int i = L; i < R; i++) {
    //        if (i <= m) { 
    //            C[i][k] = 0;
    //            Lenght[i] = Lenght[i] + 1;
    //        }
    //        else {
    //            C[i][k] = 1;
    //            Lenght[i] = Lenght[i] + 1;
    //        }
    //    }
    //    buildFano(L, m, k);
    //    buildFano(m+1, R, k);
    //}
}

boost::bimap<char, std::string> Coding::CreateShennonBM(std::vector<std::pair<char, double>> alphabet)
{
    // ��������� �� �� ����������� �������
    sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // ������ ������ ��� ������������ ������������
    std::vector<double> Q(alphabet.size() + 1, 0);

    // ����������� ������������ �����������
    for (size_t i = 1; i < Q.size(); i++) Q[i] = (alphabet[i - (size_t)1].second + Q[i - (size_t)1]);

    // ������ ������ ��� ������������ ������������,
    // ���������� ������� ����� ������������ ������������ � �������� ����
    std::vector<std::string> Qbinary(alphabet.size(), "");

    // ��������� �������� ������� ����� ������������ ������������
    // � �������� ���
    for (size_t i = 0; i < Qbinary.size(); i++) Qbinary[i] = std::get<1>(BinaryString::ftobs(Q[i]));

    // ������ ������ ���� ������� ����
    std::vector<int> L(alphabet.size(), 0);

    // ��������� ������ ������� ���� �� ������� L[i] = ceil(-log2(alphabet[i]))
    for (size_t i = 0; i < alphabet.size(); i++) L[i] = static_cast<int>(ceil(-log2(alphabet[i].second)));

    // ������ ������ ������� ����
    std::vector<std::string> keyword(alphabet.size(), "");

    // ��������� ������� ����� ��� L[i] ���������� �������� � ������� �������� ������������
    // ������������ � ������ ������
    for (size_t i = 0; i < alphabet.size(); i++) keyword[i] = Qbinary[i].substr(0, L[i]);

    // ������ ������� map ��� �������� ��� '������' : "������� �����"
    boost::bimap<char, std::string> keywordBM;

    // ��������� ��������� �� �������� � �������� �������
    for (size_t i = 0; i < alphabet.size(); i++) keywordBM.insert({ alphabet[i].first, keyword[i] });

    if (keywordBM.size() != keyword.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
        exit(15);
    }

    DisplayTable(alphabet, keywordBM, Q, Code::Shennon);

    return keywordBM;
}

boost::bimap<char, std::string> Coding::CreateHuffmanBM(std::vector<std::pair<char, double>> alphabet)
{
    //// ��������� �� �� ����������� �������
    //sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    //for (int i = 0; i < alphabet.size(); i++) {
    //    std::cout << P[i] << std::endl;
    //}

    //for (int i = 0; i < 100; i++) {
    //    CH[i] = new int[100];
    //}

    //for (int i = 0; i < 100; i++) {
    //    for (int j = 0; j < 100; j++) {
    //        CH[i][j] = 0;
    //    }
    //}

    //buildHuffman(static_cast<int>(alphabet.size()), P);

    //for (int i = 0; i < 100; i++) {
    //    for (int j = 0; j < 100; j++) {
    //        std::cout << CH[i][j] << " ";
    //    }
    //    std::cout << std::endl;
    //}

    //// ������ ������� map ��� �������� ��� '������' : "������� �����"

    //boost::bimap<char, std::string> keywordBM;

    //// ��������� ��������� �� �������� � �������� �������
    //for (size_t i = 0; i < alphabet.size(); i++) {
    //    keywordBM.insert({ alphabet[i].first, keyword[i] });
    //}

    //if (keywordBM.size() != keyword.size()) {
    //    std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
    //    exit(15);
    //}

    return boost::bimap<char, std::string>();
}

boost::bimap<char, std::string> Coding::CreateFanoBM(std::vector<std::pair<char, double>> alphabet)
{
    //// ��������� �� �� ����������� �������
    //sort(alphabet.begin(), alphabet.end(), alphabetCompValue);

    // ����������� �����������
    //for (size_t i = 0; i < alphabet.size(); i++) {
    //    alphabet[i].second /= text.size();
    //    P.push_back(alphabet[i].second);
    //}

    //for (int i = 0; i < P.size(); i++) {
    //    std::cout << P[i] << std::endl;
    //}

    //buildFano(0, P.size()-1, 0);


    //for (int i = 0; i < 100; i++) {
    //    for (int j = 0; j < 100; j++) {
    //        std::cout << C[i][j] << " ";
    //    }
    //    std::cout << std::endl;
    //}

    return boost::bimap<char, std::string>();
}

boost::bimap<char, std::string> Coding::CreateGilbertMooreBM(std::vector<std::pair<char, double>> alphabet)
{
    // ��������� �� �� ����������� �������
    sort(alphabet.begin(), alphabet.end(), alphabetCompKeyR);

    // ������ ������ ��� ������������ ������������
    std::vector<double> Q(alphabet.size() + 1, 0);

    // ����������� ������������ �����������
    for (size_t i = 1; i < Q.size(); i++) {
        Q[i] = 0;
        for (size_t j = 0; j < i; j++) {
            if (j != (i - 1)) Q[i] += alphabet[j].second;
            else Q[i] += alphabet[j].second * 0.5f;
        }
    }

    // ������ ������ ��� ������������ ������������,
    // ���������� ������� ����� ������������ ������������ � �������� ����
    std::vector<std::string> Qbinary(alphabet.size() + 1, "");

    // ��������� �������� ������� ����� ������������ ������������
    // � �������� ���
    for (size_t i = 0; i < Qbinary.size(); i++) Qbinary[i] = std::get<1>(BinaryString::ftobs(Q[i]));

    // ������ ������ ���� ������� ����
    std::vector<int> L(alphabet.size(), 0);

    // ��������� ������ ������� ���� �� ������� L[i] = ceil(-log2(alphabet[i]))
    for (size_t i = 0; i < alphabet.size(); i++) L[i] = static_cast<int>(ceil(-log2(alphabet[i].second))) + 1;

    // ������ ������ ������� ����
    std::vector<std::string> keyword(alphabet.size(), "");

    // ��������� ������� ����� ��� L[i] ���������� �������� � ������� �������� ������������
    // ������������ � ������ ������
    for (size_t i = 1; i < alphabet.size() + 1; i++) keyword[i - 1] = Qbinary[i].substr(0, L[i - 1]);

    // ������ ������� map ��� �������� ��� '������' : "������� �����"

    boost::bimap<char, std::string> keywordBM;

    // ��������� ��������� �� �������� � �������� �������
    for (size_t i = 0; i < alphabet.size(); i++) keywordBM.insert({ alphabet[i].first, keyword[i] });

    if (keywordBM.size() != keyword.size()) {
        std::cerr << std::endl << "[KEYWORD]: Keyword's are not unic!" << std::endl << std::endl;
        exit(15);
    }

    DisplayTable(alphabet, keywordBM, Q, Code::GilbertMoore);

    return keywordBM;
}
