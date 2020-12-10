#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>

#define NOT_FOUND string::npos
using namespace std;

class Vertex {
public:
    Vertex* left = nullptr;
    Vertex* right = nullptr;
    char data = {};
    double period = 0;

    Vertex(char data_, double freq_) {
        data = data_;
        period = freq_;
    }
};

namespace Comparator {
    bool charByValue(const pair<char, double>& a, const pair<char, double>& b) {
        return a.second > b.second;
    }

    bool charByKey(const pair<char, double>& a, const pair<char, double>& b) {
        return a.first <= b.first;
    }

    struct vertexByFreq {
        bool operator() (Vertex* left, Vertex* right) const {
            return left->period > right->period;
        }
    };
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

pair<string, string> floatToBinaryStr(float number) {
    int intPart = (int)(number);
    float floatPart = number - intPart;
    string beforePoint = intToBinaryStr(intPart).first;
    string afterPoint = "";
    
    for (int i = 0; i < 16; i++) {
        floatPart = floatPart * 2;
        if (floatPart >= 1) {
            afterPoint += '1';
            floatPart -= 1;
        } else {
            afterPoint += '0';
        }
    }

    return { beforePoint, afterPoint };
}

void printTab(const vector<pair<char, double>>& alphabet, const map<char, string>& symbolAndKey) {
    cout << string(55, '=');
    cout << endl << "A  " << "Pi\t\t" << "L\t" << "Keyword" << endl;
    cout << string(55, '=') << endl;
    for (int i = 0; i < alphabet.size(); i++) {
        cout << alphabet[i].first << "  " << alphabet[i].second 
             << "\t" << symbolAndKey.at(alphabet[i].first).size() 
             << "\t" << symbolAndKey.at(alphabet[i].first) << endl;
    }
    cout << string(55, '=') << endl << endl << endl;
}

int letterSearch(const vector<pair<char, double>>& textAlphabet, const char& el) {
    for (int i = 0; i < textAlphabet.size(); i++)
        if (textAlphabet[i].first == el) return i;

    return NOT_FOUND;
}

double alphabetEntropy(vector<pair<char, double>> textAlphabet) {
    double entropy = 0;

    for (int i = 0; i < textAlphabet.size(); i++)
        entropy += -textAlphabet[i].second * log2(textAlphabet[i].second);

    return entropy;
}

double averageKeywordLenght(const map<char, string> & symbolAndKey, vector<pair<char, double>> textAlphabet) {
    double averageLenght = 0;

    for (int i = 0; i < textAlphabet.size(); i++)
        averageLenght += textAlphabet[i].second * symbolAndKey.at(textAlphabet[i].first).size();

    return averageLenght;
}

namespace Encoding {
    void createShannonM(vector<pair<char, double>>& alphabet, map<char, string>& symbolAndKey) {
        vector<double> Q(alphabet.size() + 1, 0);
        vector<string> Qbinary(alphabet.size(), "");
        vector<string> keyword(alphabet.size(), "");

        sort(alphabet.begin(), alphabet.end(), Comparator::charByValue);
        
        for (int i = 1; i < Q.size(); i++) {
            Q[i] = (alphabet[i - 1].second + Q[i - 1]);
            Qbinary[i - 1] = floatToBinaryStr(Q[i - 1]).second;
            keyword[i - 1] = Qbinary[i - 1].substr(0, (int)(ceil(-log2(alphabet[i - 1].second))));

            symbolAndKey.insert({ alphabet[i - 1].first, keyword[i - 1] });
        }
    }

    void createGilbertMooreM(vector<pair<char, double>>& alphabet, map<char, string>& symbolAndKey) {
        vector<double> Q(alphabet.size(), 0);
        vector<string> Qbinary(alphabet.size(), "");
        vector<string> keyword(alphabet.size(), "");

        sort(alphabet.begin(), alphabet.end(), Comparator::charByKey);
        
        for (int i = 0; i < Q.size(); i++) {
            for (int j = 0; j < i; j++)
                Q[i] += alphabet[j].second;
            Q[i] += alphabet[i].second * 0.5f;
            Qbinary[i] = floatToBinaryStr(Q[i]).second;
            keyword[i] = Qbinary[i].substr(0, (int)((ceil(-log2(alphabet[i].second))) + 1));
        }

        for (int i = 0; i < alphabet.size(); i++) symbolAndKey.insert({ alphabet[i].first, keyword[i] });
    }

    int fanoGetMedian(vector<pair<char, double>> alphabet, int leftBorder, int rightBorder) {
        double probability = 0;
        double sum = 0;
        for (int i = leftBorder; i <= rightBorder; ++i) probability += alphabet[i].second;

        int i = 0;
        for (i = leftBorder; i < rightBorder; ++i) {
            if ((sum <= (probability / 2) && (sum + alphabet[i].second >= (probability / 2)))) {
                break;
            }
            sum += alphabet[i].second;
        }

        return i;
    }

    void createFano(vector<pair<char, double>> alphabet, map<char, string>& symbolAndKey, int leftBorder, int rightBorder) {
        if (leftBorder >= rightBorder) { return; }

        int medianIndex = Encoding::fanoGetMedian(alphabet, leftBorder, rightBorder);
        for (int i = leftBorder; i <= rightBorder; i++)
            symbolAndKey[alphabet[i].first] += to_string(int(i > medianIndex));

        createFano(alphabet, symbolAndKey, leftBorder, medianIndex);
        createFano(alphabet, symbolAndKey, medianIndex + 1, rightBorder);
    }

    void huffmanKeywordCreate(Vertex* pointer, map<char, string>& symbolAndKey, string keyword, char binarySimbol) {
        keyword.push_back(binarySimbol);

        if (pointer->left != nullptr && pointer->right != nullptr) {
            huffmanKeywordCreate(pointer->left, symbolAndKey, keyword, '0');
            huffmanKeywordCreate(pointer->right, symbolAndKey, keyword, '1');
        } else
            symbolAndKey[pointer->data] = keyword;
    }

    void createHuffman(multiset<Vertex*, Comparator::vertexByFreq>& msetOfVertexes, map<char, string>& symbolAndKey) {
        while (msetOfVertexes.size() != 1) {
            Vertex* mergedNode = new Vertex({}, (*msetOfVertexes.rbegin())->period + (*(++msetOfVertexes.rbegin()))->period);
            
            mergedNode->right = *msetOfVertexes.rbegin();
            mergedNode->left = *(++msetOfVertexes.rbegin());

            msetOfVertexes.erase(--msetOfVertexes.end());
            msetOfVertexes.erase(--msetOfVertexes.end());

            msetOfVertexes.insert(mergedNode);
        }
        huffmanKeywordCreate((*msetOfVertexes.begin())->left, symbolAndKey, "", '0');
        huffmanKeywordCreate((*msetOfVertexes.begin())->right, symbolAndKey, "", '1');
    }
}

int main() {
    system("CHCP 1251");
    system("CLS");

    string text;
    string getlineBuffer;
    ifstream openedText;
    vector<pair<char, double>> textAlphabet;

    map<char, string> ShannonM;
    map<char, string> GilbertMooreM;
    map<char, string> HuffmanM;
    map<char, string> FanoM;
    multiset<Vertex*, Comparator::vertexByFreq> alphabetTree;

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

    for (int i = 0; i < textAlphabet.size(); ++i)
        textAlphabet[i].second /= text.size();
    for (int i = 0; i < textAlphabet.size(); ++i)
        alphabetTree.insert(new Vertex(textAlphabet[i].first, textAlphabet[i].second));
    
    sort(textAlphabet.begin(), textAlphabet.end(), Comparator::charByValue);

    //Гилберт-Мур
    Encoding::createGilbertMooreM(textAlphabet, GilbertMooreM);
    cout << string(55, '=') << endl;
    cout << "GilbertMoore Table" << endl;
    printTab(textAlphabet, GilbertMooreM);

    // Шеннон
    Encoding::createShannonM(textAlphabet, ShannonM);
    cout << string(55, '=') << endl;
    cout << "Shannon Table" << endl;
    printTab(textAlphabet, ShannonM);

    // Хаффман
    Encoding::createHuffman(alphabetTree, HuffmanM);
    cout << string(55, '=') << endl;
    cout << "Huffman Table" << endl;
    printTab(textAlphabet, HuffmanM);

    // Фано
    Encoding::createFano(textAlphabet, FanoM, 0, textAlphabet.size() - 1);
    cout << string(55, '=') << endl;
    cout << "Fano Table" << endl;
    printTab(textAlphabet, FanoM);
    
    // Таблица сравнения энтропии алфавита и средней длины слова
    cout << endl << endl;
    cout << string(55, '=') << endl;
    cout << "Alphabet entrophy:\t\t\t" << alphabetEntropy(textAlphabet) << endl;
    cout << string(55, '=') << endl;
    cout << "Gilbert-Moore average code length:\t" << averageKeywordLenght(GilbertMooreM, textAlphabet) << endl;
    cout << string(55, '=') << endl;
    cout << "Shannon average code length:\t\t" << averageKeywordLenght(ShannonM, textAlphabet) << endl;
    cout << string(55, '=') << endl;
    cout << "Huffman average code length:\t\t" << averageKeywordLenght(HuffmanM, textAlphabet) << endl;
    cout << string(55, '=') << endl;
    cout << "Fano average code length:\t\t" << averageKeywordLenght(FanoM, textAlphabet) << endl;
    cout << string(55, '=') << endl;
}