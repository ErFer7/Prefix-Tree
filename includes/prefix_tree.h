// Copyright [2021] <Eric Fernandes Evaristo>
// v1.0.1

#ifndef STRUCTURES_PREFIX_TREE_H
#define STRUCTURES_PREFIX_TREE_H

#include <cstdint>    // std::size_t
#include <stdexcept>  // C++ exceptions
#include <string>

#include "array_list.h"

#define ASCII_OFFSET 97  // 97 é o código ascii da letra 'a'

using std::string;

namespace structures {

// Classe PrefixTree, árvore de prefixos
class PrefixTree {
   public:
    // Construtor
    PrefixTree();
    // Destrutor
    ~PrefixTree();
    // Insere um prefixo
    void insert(const string& prefix, unsigned long position, unsigned long length);
    // Remove um prefixo
    void remove(const string& prefix);
    // Verifica se contém um prefixo
    bool contains(const string& prefix) const;
    // Verifica se a árvore está vazia
    bool empty() const;
    // Retorna o tamanho da árvore
    std::size_t size() const;
    // Retorna uma lista de prefixos em ordem alfabética
    ArrayList<string> aphabetical_order() const;
    // Retorna o número de prefixos contidos no prefixo do parâmetro
    unsigned long prefix_search(const string& prefix) const;
    // Retorna a posição do prefixo
    unsigned long position_search(const string& prefix) const;
    // Retorna o comprimento da linha do prefixo
    unsigned long length_search(const string& prefix) const;

   private:
    // Estrutura de nó que descreve uma letra do prefixo
    struct Node {
        Node* _children[26];          // Vetor de ponteiros para cada letra
        unsigned long _position;      // Posição
        unsigned long _length;        // Comprimento
        unsigned long _prefix_count;  // Quantidade de prefixos contidos abaixo deste nó

        /**
         * Constrói uma estrutura structures::PrefixTree::Node.
         *      Parâmetros:
         *          position: Posição do nó.
         *          length: Altura da matriz.
         **/
        explicit Node(const unsigned long& position, const unsigned long& length) {
            for (int i = 0; i < 26; ++i) {
                _children[i] = nullptr;
            }

            _position = position;
            _length = length;
            _prefix_count = 0;
        }

        /**
         * Retorna a posição (unsigned long) do caractere no arquivo.
         **/
        unsigned long position() const { return _position; }

        /**
         * Define a posição (unsigned long) do caractere no arquivo com o parâmetro posição.
         **/
        void position(unsigned long position) { _position = position; }

        /**
         * Retorna o comprimento (unsigned long) da linha do prefixo.
         **/
        unsigned long length() const { return _length; }

        /**
         * Define o comprimento (unsigned long) da linha do prefixo.
         **/
        void length(unsigned long length) { _length = length; }

        /**
         * Retorna a quantidade (unsigned long) de prefixos abaixo do nó.
         **/
        unsigned long prefix_count() const { return _prefix_count; }

        /**
         * Incrementa a quantidade de prefixos abaixo do nó.
         **/
        void increase_prefix_count() { ++_prefix_count; }

        /**
         * Decrementa a quantidade de prefixos abaixo do nó.
         **/
        void decrease_prefix_count() { --_prefix_count; }

        /**
         * Insere o prefixo de forma recursiva. Cada caractere corresponde a um nó.
         *      Parâmetros:
         *          prefix: Prefíxo (string) a ser inserido.
         *          position: Posição (unsigned long) do caractere no arquivo.
         *          length: Comprimento (unsigned long) da linha do prefixo.
         *          index: Índice (std::size_t) do caractere que será inserido no nó.
         **/
        void insert(const string& prefix, const unsigned long& position,
                    const unsigned long& length, const std::size_t& index) {
            // Observação: Esse método é chamado no nó que irá inserir os dados no nó abaixo dele

            // Verifica se o índice é maior que o prefixo, caso seja isso significa que o prefixo
            // deve ser inserido no próprio nó em que este método foi chamado. Caso contrário faz
            // a inserção normal
            if (index < prefix.length()) {
                // Caso o caractere não tenha sido inserido em um nó ainda ele será criado
                if (_children[prefix[index] - ASCII_OFFSET] == nullptr) {
                    // Cria um novo nó para representar o caractere
                    _children[prefix[index] - ASCII_OFFSET] = new Node(position, length);

                    // Verifica se a alocação foi bem sucedida
                    if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                        // Caso o nó criado corresponda a um caractere intermediário do prefixo ele
                        // terá os seus dados zerados e a inserção do próximo caractere será
                        // chamada. Caso contrário o nó corresponde ao último caractere do prefixo,
                        // sendo assim a inserção se encerra e o número de prefixos contidos por
                        // aquele nó será incrementado
                        if (index < prefix.length() - 1) {
                            _children[prefix[index] - ASCII_OFFSET]->position(0);
                            _children[prefix[index] - ASCII_OFFSET]->length(0);
                            _children[prefix[index] - ASCII_OFFSET]->insert(prefix, position,
                                                                            length, index + 1);
                        } else {
                            _children[prefix[index] - ASCII_OFFSET]->increase_prefix_count();
                        }
                    } else {
                        throw std::out_of_range("Allocation Error");
                    }

                } else {  // Caso o caractere já exista chama a inserção no próximo nó
                    _children[prefix[index] - ASCII_OFFSET]->insert(prefix, position, length,
                                                                    index + 1);
                }
            } else {
                _position = position;
                _length = length;
            }

            increase_prefix_count();  // Incrementa a contagem de prefixos contidos pelo nó
        }

        /**
         * Remove o prefixo de forma recursiva. Cada caractere corresponde a um nó.
         *      Parâmetros:
         *          prefix: Prefíxo (string) a ser removido.
         *          index: Índice (std::size_t) do caractere que será removido.
         *      Retorno (bool): valor que indica se o nó abaixo foi deletado (verdadeiro) ou foi
         *      apenas zerado (falso).
         **/
        bool remove(const string& prefix, const std::size_t& index) {
            // Os nós que não tiverem filhos serão deletados, mas os nós que tiverem algum filho só
            // terão os seus dados apagados. Esse método só funciona se o prefixo existir e não pode
            // ser chamado antes da verificação da presença do nó
            if (index < prefix.length() - 1) {  // Caso o nó filho não seja o alvo de deleção
                bool deleted_node;              // Condição de deleção
                // Chama a remoção no nó filho
                deleted_node = _children[prefix[index] - ASCII_OFFSET]->remove(prefix, index + 1);

                if (deleted_node == true) {     // Se o nó filho foi deletado
                    if (prefix_count() == 1) {  // Se esse nó só possui um prefixo abaixo dele
                        // Este nó se deleta e retorna verdadeiro para a condição de deleção
                        delete this;
                        return true;
                    }
                }
            } else if (index == prefix.length() - 1) {  // Caso o nó filho seja o alvo de deleção
                // Caso o nó filho possua só um prefixo abaixo dele ele será deletado, caso
                // contrário os dados serão apenas apagados
                if (_children[prefix[index] - ASCII_OFFSET]->prefix_count() == 1) {
                    delete _children[prefix[index] - ASCII_OFFSET];     // Deleta o nó filho
                    _children[prefix[index] - ASCII_OFFSET] = nullptr;  // Define o filho como nulo

                    // Caso esse nó não seja um prefixo e não contenha mais que um prefixo abaixo
                    // dele ele irá deletar a si mesmo e retornar verdadeiro para a condição de
                    // deleção
                    if (length() == 0 && prefix_count() == 1) {
                        delete this;
                        return true;
                    }
                } else {
                    // Apaga os dados do nó filho e reduz a contagem de prefixos naquele nó
                    _children[prefix[index] - ASCII_OFFSET]->position(0);
                    _children[prefix[index] - ASCII_OFFSET]->length(0);
                    _children[prefix[index] - ASCII_OFFSET]->decrease_prefix_count();
                }
            } else {  // Caso o nó a ser deletado seja este nó
                // Este nó se deleta e retorna verdadeiro para a condição de deleção
                delete this;
                return true;
            }

            decrease_prefix_count();  // Decrementa a contagem de prefixos incluídos
            return false;             // Retorna falso para a condição de deleção
        }

        /**
         * Verifica se o prefixo está contido de forma recursiva.
         *      Parâmetros:
         *          prefix: Prefíxo (string) a ser verificado.
         *          index: Índice (std::size_t) do caractere que será verificado.
         *      Retorno (bool): valor que indica se o fim do prefixo foi encontrado ou não.
         **/
        bool contains(const string& prefix, const std::size_t& index) const {
            // Caminha pelos nós e verifica se o nó final possui um comprimento diferente de 0
            if (index < prefix.length() - 1) {  // O nó procurado está abaixo do filho
                // Verifica se o nó filho não é nulo
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    // Procura no nó filho
                    return _children[prefix[index] - ASCII_OFFSET]->contains(prefix, index + 1);
                }
            } else if (index == prefix.length() - 1) {  // O nó procurado é o filho
                // Verifica se o nó filho não é nulo
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    // Verifica se o filho tem um comprimento diferente de 0 (é prefixo) e retorna
                    // verdadeiro caso tenha
                    if (_children[prefix[index] - ASCII_OFFSET]->length() != 0) {
                        return true;
                    }
                }
            } else {  // Caso o nó procurado seja este
                // Verifica se este nó tem um comprimento diferente de 0 (é prefixo) e retorna
                // verdadeiro caso tenha
                if (length() != 0) {
                    return true;
                }
            }

            return false;  // Nó não encontrado
        }

        /**
         * Retorna uma lista com todos os prefixos abaixo deste nó (recursivamente).
         *      Parâmetros:
         *          prefix: Prefíxo (string) que está sendo construído.
         *          list: Lista (ArrayList<string>) com os prefixos.
         *          index: Índice (std::size_t) do caractere que será adicionado.
         *      Retorno (ArrayList<string>): Lista com os prefixos em ordem alfabética.
         **/
        void alphabetical_order(const string& prefix, ArrayList<string>& list,
                                const std::size_t& index) const {
            // Cria um novo string para o prefixo. Essa cópia é feita para permitir a ramificação
            // dos prefixos com diferentes origens
            string new_prefix(prefix);
            // Adiciona o caractere deste nó no prefixo
            new_prefix.push_back(char(index + ASCII_OFFSET));

            // Se o este nó é o fim de um prefixo, adiciona o prefixo na lista
            if (length() != 0) {
                list.push_back(new_prefix);
            }

            // Procura por nós filhos e chama o método recursivamente
            for (int i = 0; i < 26; ++i) {
                if (_children[i] != nullptr) {
                    _children[i]->alphabetical_order(new_prefix, list, i);
                }
            }
        }

        /**
         * Retorna o número de prefixos abaixo do nó (recursivamente).
         *      Parâmetros:
         *          prefix: Prefíxo (string) que está sendo procurado.
         *          index: Índice (std::size_t) do caractere do prefixo.
         *      Retorno (unsigned long): Número de prefixos contidos abaixo do nó.
         **/
        unsigned long prefix_search(const string& prefix, const std::size_t& index) {
            // Este método caminha entre os nós até encontrar o último caractere do prefixo ou até
            // que acabem os nós no caminho feito
            if (index < prefix.length() - 1) {  // Caso o nó filho não seja o procurado
                // Verifica se o filho não é nulo e chama a pesquisa recursivamente
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->prefix_search(prefix,
                                                                                  index + 1);
                }
            } else if (index == prefix.length() - 1) {  // Caso o nó filho seja o procurado
                // Verifica se o filho não é nulo e obtém o valor de prefixos contidos no filho
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->prefix_count();
                }
            } else {  // Caso este nó seja o procurado (Caso que o prefixo tem só um caractere)
                return prefix_count();  // Retorna a quantidade de prefixos contidos neste prefixo
            }

            return 0;  // Caso o prefixo procurado não tenha sido encontrado
        }

        /**
         * Retorna a posição do nó encontrado na pesquisa (recursivamente).
         *      Parâmetros:
         *          prefix: Prefíxo (string) que está sendo procurado.
         *          index: Índice (std::size_t) do caractere do prefixo.
         *      Retorno (unsigned long): Posição do nó encontrado (-1 caso não seja encontrado).
         **/
        unsigned long position_search(const string& prefix, const std::size_t& index) {
            // Este método caminha entre os nós até encontrar o último caractere do prefixo ou até
            // que acabem os nós no caminho feito
            if (index < prefix.length() - 1) {  // Caso o nó filho não seja o procurado
                // Verifica se o filho não é nulo e chama a pesquisa recursivamente
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->position_search(prefix,
                                                                                    index + 1);
                }
            } else if (index == prefix.length() - 1) {  // Caso o nó filho seja o procurado
                // Verifica se o filho não é nulo
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    // Verifica se o filho tem um comprimento diferente de 0 (é o fim de um prefixo)
                    if (_children[prefix[index] - ASCII_OFFSET]->length() != 0) {
                        // Retorna a posição do filho
                        return _children[prefix[index] - ASCII_OFFSET]->position();
                    }
                }
            } else if (length() != 0) {  // Caso este nó seja o procurado e seja o fim de um prefixo
                return position();       // Retorna a posição
            }

            return -1;  // Caso o prefixo procurado não tenha sido encontrado
        }

        /**
         * Retorna o comprimento do nó encontrado na pesquisa (recursivamente).
         *      Parâmetros:
         *          prefix: Prefíxo (string) que está sendo procurado.
         *          index: Índice (std::size_t) do caractere do prefixo.
         *      Retorno (unsigned long): Comprimento do nó encontrado (0 caso não seja encontrado).
         **/
        unsigned long length_search(const string& prefix, const std::size_t& index) {
            // Este método caminha entre os nós até encontrar o último caractere do prefixo ou até
            // que acabem os nós no caminho feito
            if (index < prefix.length() - 1) {  // Caso o nó filho não seja o procurado
                // Verifica se o filho não é nulo e chama a pesquisa recursivamente
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->length_search(prefix,
                                                                                  index + 1);
                }
            } else if (index == prefix.length() - 1) {  // Caso o nó filho seja o procurado
                // Caso o filho não seja nulo, retorna o comprimento do filho
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->length();
                }
            } else {              // Caso este nó seja o procurado
                return length();  // Retorna o comprimento
            }

            return 0;  // Caso o prefixo procurado não tenha sido encontrado
        }
    };

    Node* _root[26];    // Raiz
    std::size_t _size;  // Tamanho da árvore
};

}  // namespace structures

/**
 * Constrói um objeto structures::PrefixTree.
 **/
structures::PrefixTree::PrefixTree() {
    // Inicializa os atributos
    for (int i = 0; i < 26; ++i) {
        _root[i] = nullptr;
    }

    _size = 0u;
}

/**
 * Destrói o objeto structures::PrefixTree.
 **/
structures::PrefixTree::~PrefixTree() {
    // Gera uma lista com todos os prefixos e remove todos eles
    structures::ArrayList<string> list = aphabetical_order();
    for (std::size_t i = 0; i < list.size(); ++i) {
        remove(list.at(i));
    }
}

/**
 * Insere o prefixo. Cada caractere corresponde a um nó.
 *      Parâmetros:
 *          prefix: Prefíxo (string) a ser inserido.
 *          position: Posição (unsigned long) do caractere no arquivo.
 *          length: Comprimento (unsigned long) da linha do prefixo.
 **/
void structures::PrefixTree::insert(const string& prefix, unsigned long position,
                                    unsigned long length) {
    if (_root[prefix[0] - ASCII_OFFSET] == nullptr) {  // Checa se o filho na raiz é nulo
        _root[prefix[0] - ASCII_OFFSET] = new Node(position, length);  // Cria o novo nó
        if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {  // Verifica se a alocação deu certo
            if (prefix.length() > 1) {  // Caso o prefixo tenha mais de um caractere
                // Zera os atributos
                _root[prefix[0] - ASCII_OFFSET]->position(0);
                _root[prefix[0] - ASCII_OFFSET]->length(0);
                // Inserção recursiva
                _root[prefix[0] - ASCII_OFFSET]->insert(prefix, position, length, 1);
            } else {
                // Incrementa a contagem de prefixos contidos
                _root[prefix[0] - ASCII_OFFSET]->increase_prefix_count();
            }
        } else {
            throw std::out_of_range("Allocation Error");
        }
    } else {
        // Inserção recursiva
        _root[prefix[0] - ASCII_OFFSET]->insert(prefix, position, length, 1);
    }

    ++_size;  // Incrementa o tamanho
}

/**
 * Remove o prefixo.
 *      Parâmetros:
 *          prefix: Prefíxo (string) a ser removido.
 **/
void structures::PrefixTree::remove(const string& prefix) {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {  // Checa se o filho na raiz é nulo
        if (contains(prefix)) {                        // Checa se o prefixo está na árvore
            // Remove o prefixo
            bool deleted_node = _root[prefix[0] - ASCII_OFFSET]->remove(prefix, 1);
            // Caso o nó filho tenha sido deletado o ponteiro apontará para nulo
            if (deleted_node) {
                _root[prefix[0] - ASCII_OFFSET] = nullptr;
            }
        } else {
            throw std::out_of_range("Prefix not found");
        }
    } else {
        throw std::out_of_range("Prefix not found");
    }
}

/**
 * Verifica se o prefixo está contido.
 *      Parâmetros:
 *          prefix: Prefíxo (string) a ser verificado.
 *      Retorno (bool): valor que indica se o fim do prefixo foi encontrado ou não.
 **/
bool structures::PrefixTree::contains(const string& prefix) const {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {  // Checa se o filho da raiz não é nulo
        // Verificação recursiva
        return _root[prefix[0] - ASCII_OFFSET]->contains(prefix, 1);
    } else {
        // Caso o filho da raiz seja nulo o prefixo não está contido
        return false;
    }
}

/**
 * Retorna verdadeiro caso a árvore esteja vazia.
 **/
bool structures::PrefixTree::empty() const { return size() == 0; }

/**
 * Retorna o tamanho (std::size_t).
 **/
std::size_t structures::PrefixTree::size() const { return _size; }

/**
 * Retorna uma lista (ArrayList<string>) com todos os prefixos em ordem alfabética.
 **/
structures::ArrayList<string> structures::PrefixTree::aphabetical_order() const {
    structures::ArrayList<string> list(size());  // Cria a lista

    if (!empty()) {                     // Caso a árvore não esteja vazia
        string prefix;                  // String para o prefixo
        for (int i = 0; i < 26; ++i) {  // Itera pelos filhos da raiz
            if (_root[i] != nullptr) {  // Caso o filho não seja nulo
                // Ordem alfabética recursiva
                _root[i]->alphabetical_order(prefix, list, i);
            }
        }
    }

    return list;  // Retorna a lista
}

/**
 * Retorna o número de prefixos contidos em um prefixo.
 *      Parâmetros:
 *          prefix: Prefíxo (string) que está sendo procurado.
 *      Retorno (unsigned long): Número de prefixos contidos em um prefixo.
 **/
unsigned long structures::PrefixTree::prefix_search(const string& prefix) const {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {  // Caso o filho do nó não seja nulo
        // Pesquisa recursiva
        return _root[prefix[0] - ASCII_OFFSET]->prefix_search(prefix, 1);
    } else {
        return 0;  // Nenhum prefixo encontrado
    }
}

/**
 * Retorna a posição do nó encontrado na pesquisa.
 *      Parâmetros:
 *          prefix: Prefíxo (string) que está sendo procurado.
 *      Retorno (unsigned long): Posição do nó encontrado (-1 caso não seja encontrado).
 **/
unsigned long structures::PrefixTree::position_search(const string& prefix) const {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {  // Caso o filho do nó não seja nulo
        // Pesquisa recursiva
        return _root[prefix[0] - ASCII_OFFSET]->position_search(prefix, 1);
    } else {
        return 0;  // Nenhum prefixo encontrado
    }
}

/**
 * Retorna o comprimento do nó encontrado na pesquisa.
 *      Parâmetros:
 *          prefix: Prefíxo (string) que está sendo procurado.
 *      Retorno (unsigned long): Comprimento do nó encontrado (0 caso não seja encontrado).
 **/
unsigned long structures::PrefixTree::length_search(const string& prefix) const {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {  // Caso o filho do nó não seja nulo
        // Pesquisa recursiva
        return _root[prefix[0] - ASCII_OFFSET]->length_search(prefix, 1);
    } else {
        return 0;  // Nenhum prefixo encontrado
    }
}

#endif
