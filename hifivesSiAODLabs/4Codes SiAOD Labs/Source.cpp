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
    double freq = 0;

    Vertex(char data_, double freq_) {
        data = data_;
        freq = freq_;
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
            return left->freq > right->freq;
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
    
    // 16 - number of simbols after point
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

void printTab(const vector<pair<char, double>>& alphabet, const map<char, string>& keywordM) {
    cout << endl << "A  " << "Pi\t\t" << "L\t" << "Keyword" << endl;
    cout << string(55, '=') << endl;
    for (int i = 0; i < alphabet.size(); i++) {
        cout << alphabet[i].first << "  " << alphabet[i].second 
             << "\t" << keywordM.at(alphabet[i].first).size() 
             << "\t" << keywordM.at(alphabet[i].first) << endl;
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

double averageKeywordLenght(const map<char, string> & keywordM, vector<pair<char, double>> textAlphabet) {
    double averageLenght = 0;

    for (int i = 0; i < textAlphabet.size(); i++)
        averageLenght += textAlphabet[i].second * keywordM.at(textAlphabet[i].first).size();

    return averageLenght;
}

namespace Encoding {
    map<char, string> createShennonM(vector<pair<char, double>> alphabet) {
        vector<double> Q(alphabet.size() + 1, 0);
        vector<string> Qbinary(alphabet.size(), "");
        vector<string> keyword(alphabet.size(), "");
        map<char, string> keywordM;

        sort(alphabet.begin(), alphabet.end(), Comparator::charByValue);
        
        for (int i = 1; i < Q.size(); i++) {
            Q[i] = (alphabet[i - 1].second + Q[i - 1]);
            Qbinary[i - 1] = floatToBinaryStr(Q[i - 1]).second;
            keyword[i - 1] = Qbinary[i - 1].substr(0, (int)(ceil(-log2(alphabet[i - 1].second))));

            keywordM.insert({ alphabet[i - 1].first, keyword[i - 1] });
        }

        cout << string(55, '=') << endl;
        cout << "Shannon Table" << endl;
        cout << string(55, '=');
        printTab(alphabet, keywordM);

        return keywordM;
    }

    map<char, string> createGilbertMooreM(vector<pair<char, double>> alphabet) {
        vector<double> Q(alphabet.size(), 0);
        vector<string> Qbinary(alphabet.size(), "");
        vector<string> keyword(alphabet.size(), "");
        map<char, string> keywordM;

        sort(alphabet.begin(), alphabet.end(), Comparator::charByKey);
        
        for (int i = 0; i < Q.size(); i++) {
            for (int j = 0; j < i; j++)
                Q[i] += alphabet[j].second;
            Q[i] += alphabet[i].second * 0.5f;
            Qbinary[i] = floatToBinaryStr(Q[i]).second;
            keyword[i] = Qbinary[i].substr(0, (int)((ceil(-log2(alphabet[i].second))) + 1));
        }

        for (int i = 0; i < alphabet.size(); i++) keywordM.insert({ alphabet[i].first, keyword[i] });

        cout << string(55, '=') << endl;
        cout << "GilbertMoore Table" << endl;
        cout << string(55, '=');
        printTab(alphabet, keywordM);

        return keywordM;
    }

    int fanoGetMedian(int L, int R, const std::vector<std::pair<char, double>>& alphabet) {
        double probability = 0;
        double sum = 0;
        for (int i = L; i <= R; ++i) probability += alphabet[i].second;

        int i = 0;
        for (i = L; i < R; ++i) {
            if ((sum <= (probability / 2) && (sum + alphabet[i].second >= (probability / 2)))) {
                break;
            }
            sum += alphabet[i].second;
        }

        return i;
    }

    void createFano(int L, int R, const vector<pair<char, double>>& alphabet, map<char, string>& keywordM) {
        if (L >= R) { return; }

        int medianIndex = Encoding::fanoGetMedian(L, R, alphabet);
        for (int i = L; i <= R; i++) { keywordM[alphabet[i].first] += to_string(int(i > medianIndex)); }

        createFano(L, medianIndex, alphabet, keywordM);
        createFano(medianIndex + 1, R, alphabet, keywordM);
    }

    void huffmanBuildKeywords(Vertex* p, map<char, string>& keywordM, string keyword, char bit) {
        keyword.push_back(bit);

        if (p->left != nullptr && p->right != nullptr) {
            huffmanBuildKeywords(p->left, keywordM, keyword, '0');
            huffmanBuildKeywords(p->right, keywordM, keyword, '1');
        }
        else
            keywordM[p->data] = keyword;
    }

    void createHuffman(multiset<Vertex*, Comparator::vertexByFreq>& alphabet) {
        while (alphabet.size() > 1) {
            Vertex* mergedNode = new Vertex(char(), (*alphabet.rbegin())->freq + (*(++alphabet.rbegin()))->freq);
            
            mergedNode->right = *alphabet.rbegin();
            mergedNode->left = *(++alphabet.rbegin());

            alphabet.erase(--alphabet.end());
            alphabet.erase(--alphabet.end());

            alphabet.insert(mergedNode);
        }
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
        
        if (letter != NOT_FOUND) {
            textAlphabet[letter].second += 1.0;
        } else {
            textAlphabet.push_back(make_pair(text[i], 1.0));
        }
    }

    for (int i = 0; i < textAlphabet.size(); i++)
        textAlphabet[i].second /= text.size();
    for (const auto& letter : textAlphabet)
        alphabetTree.insert(new Vertex(letter.first, letter.second));

    sort(textAlphabet.begin(), textAlphabet.end(), Comparator::charByValue);

    //Гилберт-Мур
    GilbertMooreM = Encoding::createGilbertMooreM(textAlphabet);
    // Шеннон
    ShannonM = Encoding::createShennonM(textAlphabet);
    // Хаффман
    Encoding::createHuffman(alphabetTree);
    if ((*alphabetTree.begin()) != nullptr) {
        if ((*alphabetTree.begin())->left != nullptr && (*alphabetTree.begin())->right != nullptr) {
            Encoding::huffmanBuildKeywords((*alphabetTree.begin())->left, HuffmanM, "", '0');
            Encoding::huffmanBuildKeywords((*alphabetTree.begin())->right, HuffmanM, "", '1');
        }
    }
    //KeywordBuildingSetup(, HuffmanM);
        // Pi порядок
    cout << string(55, '=') << endl;
    cout << "Huffman Table" << endl;
    cout << string(55, '=');
    printTab(textAlphabet, HuffmanM);
        // Лексикографический порядок
    //for (auto el = HuffmanM.begin(); el != HuffmanM.end(); el++)
        //cout << el->first << "\t" << el->second << endl;
    // Фано
    for (auto& el : textAlphabet) FanoM.insert({ el.first, "" });
    Encoding::createFano(0, textAlphabet.size() - 1, textAlphabet, FanoM);
    cout << string(55, '=') << endl;
    cout << "Fano Table" << endl;
    cout << string(55, '=');
        // Pi порядок
    printTab(textAlphabet, FanoM);
        // Лексикографический порядок
    //for (auto& el : textAlphabet)
        //cout << el.first << " " << FanoM.at(el.first) << endl;

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