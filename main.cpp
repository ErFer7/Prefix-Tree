#include <iostream>
#include <string>
#include <prefix_tree.h>

using namespace std;
using structures::PrefixTree;
using structures::ArrayList;

int main() {
    string filename;
    string word;
    PrefixTree prefix_tree;

    // cin >> filename;  // entrada

    // cout << filename << endl;  // esta linha deve ser removida

    // while (1) {  // leitura das palavras ate' encontrar "0"
    //     cin >> word;
    //     if (word.compare("0") == 0) {
    //         break;
    //     }
    //     cout << word << endl;
    // }

    prefix_tree.insert("bear", 1, 1);
    prefix_tree.insert("bell", 2, 1);
    prefix_tree.insert("bid", 3, 1);
    prefix_tree.insert("buy", 4, 1);
    prefix_tree.insert("sell", 4, 1);
    prefix_tree.insert("stock", 4, 1);
    prefix_tree.insert("stop", 4, 1);

    return 0;
}
