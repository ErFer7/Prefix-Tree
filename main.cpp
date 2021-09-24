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

    dicFile.open(filename, ios_base::binary);

    if (dicFile.is_open()) {
        bool reading_prefix = false;
        string line;
        string prefix = "";
        unsigned long position = 0;

        while (getline(dicFile, line)) {
            for (std::size_t i = 0; i < line.size(); ++i) {
                switch (line[i]) {
                    case '[':
                        reading_prefix = true;
                        break;
                    case ']':
                        reading_prefix = false;
                        break;
                    default:
                        if (reading_prefix) {
                            prefix.push_back(line[i]);
                        }
                        break;
                }
            }

            prefix_tree.insert(prefix, position, line.size());
            prefix.clear();
            position += line.size() + 1;
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
