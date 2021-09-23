#include <iostream>
#include <prefix_tree.h>

using namespace std;
using structures::PrefixTree;

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

    prefix_tree.insert("abc", 1, 1);
    cout << "Contains abc:" << prefix_tree.contains("abc") << endl;
    prefix_tree.remove("abc");
    cout << "Contains abc:" << prefix_tree.contains("abc") << endl;

    return 0;
}
