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

    unsigned long prefix_search(const string& prefix) const;

    unsigned long position_search(const string& prefix) const;

    unsigned long length_search(const string& prefix) const;

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

        unsigned long prefix_count() const { return _prefix_count; }

        void increase_prefix_count() { ++_prefix_count; }

        void decrease_prefix_count() { --_prefix_count; }

        // Insere o dado no nó filho
        void insert(const string& prefix, const unsigned long& position,
                    const unsigned long& length, const std::size_t& index) {
            if (index < prefix.length()) {
                if (_children[prefix[index] - ASCII_OFFSET] == nullptr) {
                    _children[prefix[index] - ASCII_OFFSET] = new Node(position, length);
                    //cout << "["<< this << "]: Node [" << _children[prefix[index] - ASCII_OFFSET] << "] created with char [" << prefix[index] - ASCII_OFFSET << "]" << endl;

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
        bool remove(const string& prefix, const std::size_t& index) {
            if (index < prefix.length() - 1) {
                bool deleted_node;
                deleted_node = _children[prefix[index] - ASCII_OFFSET]->remove(prefix, index + 1);

                if (deleted_node == true) {
                    if (prefix_count() == 1) {
                        delete this;
                        return true;
                    }
                }
            } else {
                if (_children[prefix[index] - ASCII_OFFSET]->prefix_count() == 1) {
                    delete _children[prefix[index] - ASCII_OFFSET];
                    _children[prefix[index] - ASCII_OFFSET] = nullptr;

                    if (length() == 0 && prefix_count() == 1) {
                        delete this;
                        return true;
                    }
                } else {
                    _children[prefix[index] - ASCII_OFFSET]->position(0);
                    _children[prefix[index] - ASCII_OFFSET]->length(0);
                    _children[prefix[index] - ASCII_OFFSET]->decrease_prefix_count();
                }
            }

            decrease_prefix_count();
            return false;
        }

        bool contains(const string& prefix, const std::size_t& index) const {
            if (index < prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->contains(prefix, index + 1);
                }
            } else if (index == prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    if (_children[prefix[index] - ASCII_OFFSET]->length() != 0) {
                        return true;
                    }
                }
            } else {
                return true;
            }
            return false;
        }

        void alphabetical_order(const string& prefix, ArrayList<string>& list, const std::size_t& index) const {
            string new_prefix(prefix);
            new_prefix.push_back(char(index + ASCII_OFFSET));

            if (length() != 0) {
                list.push_back(new_prefix);
            }

            for (int i = 0; i < 26; ++i) {
                if (_children[i] != nullptr) {
                    _children[i]->alphabetical_order(new_prefix, list, i);
                }
            }
        }

        unsigned long prefix_search(const string& prefix, const std::size_t& index) {
            if (index < prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->prefix_search(prefix, index + 1);
                }
            } else if (index == prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->prefix_count();
                }
            } else {
                return prefix_count();
            }

            return 0;
        }

        unsigned long position_search(const string& prefix, const std::size_t& index) {
            if (index < prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->position_search(prefix, index + 1);
                }
            } else if (index == prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    if (_children[prefix[index] - ASCII_OFFSET]->length() != 0) {
                        return _children[prefix[index] - ASCII_OFFSET]->position();
                    }
                }
            } else if (length() != 0) {
                return position();
            }

            return -1;
        }

        unsigned long length_search(const string& prefix, const std::size_t& index) {
            if (index < prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    return _children[prefix[index] - ASCII_OFFSET]->length_search(prefix, index + 1);
                }
            } else if (index == prefix.length() - 1) {
                if (_children[prefix[index] - ASCII_OFFSET] != nullptr) {
                    if (_children[prefix[index] - ASCII_OFFSET]->length() != 0) {
                        return _children[prefix[index] - ASCII_OFFSET]->length();
                    }
                }
            } else if (length() != 0) {
                return length();
            }

            return 0;
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
    //cout << "Inserting [" << prefix << "]" << endl;
    if (_root[prefix[0] - ASCII_OFFSET] == nullptr) {
        _root[prefix[0] - ASCII_OFFSET] = new Node(position, length);
        //cout << "Node [" << _root[prefix[0] - ASCII_OFFSET] << "] created with char [" << prefix[0] - ASCII_OFFSET << "]" << endl;
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
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {
        if (contains(prefix)) {
            bool deleted_node = _root[prefix[0] - ASCII_OFFSET]->remove(prefix, 1);
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

bool structures::PrefixTree::contains(const string& prefix) const {
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
        string prefix;
        for (int i = 0; i < 26; ++i) {
            if (_root[i] != nullptr) {
                _root[i]->alphabetical_order(prefix, list, i);
            }
        }
    }

    return list;
}

unsigned long structures::PrefixTree::prefix_search(const string& prefix) const {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {
        return _root[prefix[0] - ASCII_OFFSET]->prefix_search(prefix, 1);
    } else {
        return 0;
    }
}

unsigned long structures::PrefixTree::position_search(const string& prefix) const {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {
        return _root[prefix[0] - ASCII_OFFSET]->position_search(prefix, 1);
    } else {
        return -1;
    }
}

unsigned long structures::PrefixTree::length_search(const string& prefix) const {
    if (_root[prefix[0] - ASCII_OFFSET] != nullptr) {
        return _root[prefix[0] - ASCII_OFFSET]->length_search(prefix, 1);
    } else {
        return 0;
    }
}
