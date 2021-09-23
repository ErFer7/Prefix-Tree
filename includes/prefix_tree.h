// Copyright [2021] <Eric Fernandes Evaristo>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <iostream>  // DEBUG

#include "array_list.h"

#define ASCII_OFFSET 97

using std::string;
// DEBUG
using std::cout;
using std::endl;

namespace structures {

class PrefixTree {
   public:
    // Construtor
    PrefixTree();
    // Destrutor
    ~PrefixTree();
    // Insere dado
    void insert(const string& prefix, unsigned long position, unsigned long length);
    // Remove dado
    void remove(const string& prefix);
    // Verifica se contém
    bool contains(const string& prefix) const;
    // Verifica se a árvore está vazia
    bool empty() const;
    // Retorna o tamanho
    std::size_t size() const;

    ArrayList<string> aphabetical_order() const;

   private:
    struct Node {
        Node* _children[26];
        unsigned long _position;
        unsigned long _length;
        unsigned long _prefix_count;

        // Constrói o nó
        explicit Node(const unsigned long& position, const unsigned long& length) {
            for (int i = 0; i < 26; ++i) {
                _children[i] = nullptr;
            }

            _position = position;
            _length = length;
            _prefix_count = 0;
        }

        unsigned long position() const { return _position; }

        void position(unsigned long position) { _position = position; }

        unsigned long length() const { return _length; }

        void length(unsigned long length) { _length = length; }

        void increase_prefix_count() { ++_prefix_count; }

        // Insere o dado no nó filho
        void insert(const string& prefix, const unsigned long& position,
                    const unsigned long& length, const std::size_t& index) {
            if (index < prefix.length()) {
                if (_children[prefix[index] - ASCII_OFFSET] == nullptr) {
                    _children[prefix[index] - ASCII_OFFSET] = new Node(position, length);
                    cout << "["<< this << "]: Node [" << _children[prefix[index] - ASCII_OFFSET] << "] created with char [" << prefix[index] - ASCII_OFFSET << "] as header" << endl;

                    if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
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

                    cout << "["<< this << "]: Node [" << _children[prefix[index] - ASCII_OFFSET] << "] has position [" << position << "]" << endl;
                } else {
                    _children[prefix[index] - ASCII_OFFSET]->insert(prefix, position, length,
                                                                    index + 1);
                }
            } else {
                _position = position;
                _length = length;
            }

            increase_prefix_count();
        }

        // Remove o nó com o dado
        Node* remove(const string& prefix, const std::size_t& index) {
            if (index < prefix.length() - 1) {
                Node* returned_node;
                returned_node = _children[prefix[index] - ASCII_OFFSET]->remove(prefix, index + 1);

                if (returned_node == nullptr) {
                    if (_prefix_count == 1) {
                        delete this;
                        return nullptr;
                    } else {
                        _children[prefix[index] - ASCII_OFFSET] = nullptr;
                    }
                } else {
                    // Inútil
                    _children[prefix[index] - ASCII_OFFSET] = returned_node;
                }
            } else {
                if (_prefix_count == 1) {
                    delete this;
                    return nullptr;
                } else {
                    position(0);
                    length(0);
                }
            }

            --_prefix_count;
            return this;
        }

        bool contains(const string& prefix, const std::size_t& index) const {
            if (index < prefix.length() - 1) {
                cout << "[" << this << "]: Node [" << _children[prefix[index] - ASCII_OFFSET] << "] with header [" << prefix[index] - ASCII_OFFSET << "] will search" << endl;
                return _children[prefix[index] - ASCII_OFFSET]->contains(prefix, index + 1);
            } else if (index == prefix.length() - 1 && _children[prefix[index] - ASCII_OFFSET] != nullptr) {
                cout << "[" << this << "]: Node found, prefix [" << prefix << "], index [" << index << "]" << endl;
                if (_children[prefix[index] - ASCII_OFFSET]->position() != 0) {
                    return true;
                }
            }
            cout << "[" << this << "]: Node not found, prefix [" << prefix << "], index [" << index << "]" << endl;
            return false;
        }

        string children() {
            string children_chars;

            for (int i = 0; i < 26; ++i) {
                if (_children[i] != nullptr) {
                    children_chars.push_back(char(i + ASCII_OFFSET));
                }
            }

            return children_chars;
        }
    };

    Node* _root[26];
    std::size_t _size;
};

}  // namespace structures

structures::PrefixTree::PrefixTree() {
    // Inicializa os atributos
    for (int i = 0; i < 26; ++i) {
        _root[i] = nullptr;
    }
    _size = 0u;
}

structures::PrefixTree::~PrefixTree() {
    structures::ArrayList<string> list = aphabetical_order();
    for (std::size_t i = 0; i < list.size(); ++i) {
        remove(list.at(i));
    }
}

void structures::PrefixTree::insert(const string& prefix, unsigned long position,
                                    unsigned long length) {
    cout << "Inserting [" << prefix << "]" << endl;
    if (_root[prefix[0] - ASCII_OFFSET] == nullptr) {
        _root[prefix[0] - ASCII_OFFSET] = new Node(position, length);
        cout << "Node [" << _root[prefix[0] - ASCII_OFFSET] << "] created with char [" << prefix[0] - ASCII_OFFSET << "] as header" << endl;
        if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {
            if (prefix.length() > 1) {
                _root[prefix[0] - ASCII_OFFSET]->position(0);
                _root[prefix[0] - ASCII_OFFSET]->length(0);
                _root[prefix[0] - ASCII_OFFSET]->insert(prefix, position, length, 1);
            } else {
                _root[prefix[0] - ASCII_OFFSET]->increase_prefix_count();
            }
        } else {
            throw std::out_of_range("Allocation Error");
        }
    } else {
        _root[prefix[0] - ASCII_OFFSET]->insert(prefix, position, length, 1);
    }

    ++_size;
}

void structures::PrefixTree::remove(const string& prefix) {
    cout << "Removing [" << prefix << "]" << endl;
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {
        if (contains(prefix)) {
            _root[prefix[0] - ASCII_OFFSET] = _root[prefix[0] - ASCII_OFFSET]->remove(prefix, 1);
        } else {
            throw std::out_of_range("Prefix not found");
        }
    } else {
        throw std::out_of_range("Prefix not found");
    }
}

bool structures::PrefixTree::contains(const string& prefix) const {
    cout << "Checking [" << prefix << "]" << endl;
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {

        return _root[prefix[0] - ASCII_OFFSET]->contains(prefix, 1);
    } else {
        return false;
    }
}

bool structures::PrefixTree::empty() const { return size() == 0; }

std::size_t structures::PrefixTree::size() const { return _size; }

structures::ArrayList<string> structures::PrefixTree::aphabetical_order() const {
    structures::ArrayList<string> list(size());
    if (!empty()) {
        for (int i = 0; i < 26; ++i) {
            if (_root[i] != nullptr) {
                list.push_back(_root[i]->children());
            }
        }
    }
    return list;
}
