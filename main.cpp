#include <prefix_tree.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using structures::ArrayList;
using structures::PrefixTree;

int main() {
    string filename;
    string word;
    PrefixTree prefix_tree;
    std::ifstream dicFile;  // Arquivo que será lido

    cin >> filename;  // entrada

    dicFile.open(filename);

    if (dicFile.is_open()) {
        char c;
        bool reading_prefix = false;
        string prefix = "";
        unsigned long current_position = 0;
        unsigned long position = 0;
        unsigned long length = 0;

        while (dicFile.get(c)) {
            switch (c) {
                case '[':
                    reading_prefix = true;
                    length = 0;
                    current_position = position;
                    break;
                case ']':
                    reading_prefix = false;
                    break;
                case '\n':
                    prefix_tree.insert(prefix, current_position, length);
                    prefix.clear();
                    break;
                default:
                    if (reading_prefix) {
                        prefix.push_back(c);
                    }
                    break;
            }

            ++length;
            ++position;
        }

        if (prefix.length() > 0) {
            prefix_tree.insert(prefix, current_position, length);
            prefix.clear();
        }

        dicFile.close();
    } else {
        throw std::out_of_range("File not found");
    }

    unsigned long prefix_count;
    unsigned long position;
    unsigned long length;

    while (1) {  // leitura das palavras ate' encontrar "0"
        cin >> word;

        if (word.compare("0") == 0) {
            break;
        }

        prefix_count = prefix_tree.prefix_search(word);

        if (prefix_count > 0) {
            cout << word << " is prefix of " << prefix_count << " words" << endl;

            position = prefix_tree.position_search(word);
            length = prefix_tree.length_search(word);

            if (length != 0) {
                cout << word << " is at (" << position << "," << length << ")" << endl;
            }
        } else {
            cout << word << " is not prefix" << endl;
        }

    }

    return 0;
}
