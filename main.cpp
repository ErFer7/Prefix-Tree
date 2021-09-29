// Copyright [2021] <Eric Fernandes Evaristo>
// v1.0

#include <prefix_tree.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using structures::ArrayList;
using structures::PrefixTree;

int main() {
    string filename;         // Nome do arquivo
    string word;             // Palavra a ser pesquisada
    PrefixTree prefix_tree;  // Árvore de prefixos
    std::ifstream dicFile;   // Arquivo que será lido

    cin >> filename;  // Entrada do nome do arquivo

    dicFile.open(filename);  // Tenta abrir o arquivo

    // Se o arquivo está aberto o seu conteúdo é lido e árvore de prefixos é construída
    if (dicFile.is_open()) {
        bool reading_prefix;         // Booleano de estado de leitura
        string line;                 // String para a linha
        string prefix = "";          // String para o prefixo
        unsigned long position = 0;  // Posição do caractere

        // Enquanto não for o fim do texto, lê linha por linha
        while (getline(dicFile, line)) {
            // Itera por cada caractere na linha
            for (std::size_t i = 0; i < line.size(); ++i) {
                // No primeiro caractere o estado de leitura é definido como verdadeiro. Na leitura
                // o caractere é adicionado no string do prefixo enquanto o caractere for válido.
                // Assim que o caractere não for válido o estado de leitura é redefinido como falso
                if (i == 0) {
                    reading_prefix = true;
                } else if (reading_prefix && (line[i] >= 'a' && line[i] <= 'z')) {
                    prefix.push_back(line[i]);
                } else {
                    reading_prefix = false;
                }
            }

            prefix_tree.insert(prefix, position, line.size());  // Insere o prefixo na árvore
            prefix.clear();                                     // Limpa o string de prefixos
            position += line.size() + 1;                        // Calcula a posição
        }

        dicFile.close();  // Fecha o arquivo
    } else {
        throw std::out_of_range("File not found");
    }

    unsigned long prefix_count;  // Quantidade de prefixos contidos em um prefixo
    unsigned long position;      // Posição do prefixo no arquivo
    unsigned long length;        // Comprimento da linha

    while (1) {  // leitura das palavras até encontrar "0"
        cin >> word;

        if (word.compare("0") == 0) {
            break;
        }

        // Obtém a quantidade de prefixos contidos na palavra
        prefix_count = prefix_tree.prefix_search(word);

        if (prefix_count > 0) {  // Existem prefixos contidos na palavra
            // Exibe a quantidade de prefixos contidos
            cout << word << " is prefix of " << prefix_count << " words" << endl;

            position = prefix_tree.position_search(word);  // Obtém a posição do prefixo exato
            length = prefix_tree.length_search(word);      // Obtém o comprimento do prefixo exato

            // Caso a palavra corresponda a um prefixo exato, a posição e o comprimento serão
            // exibidos
            if (length != 0) {
                cout << word << " is at (" << position << "," << length << ")" << endl;
            }
        } else {  // Não há nenhum prefixo contido na palavra
            cout << word << " is not prefix" << endl;
        }
    }

    return 0;
}
