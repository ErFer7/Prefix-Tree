// Copyright [2021] <Eric Fernandes Evaristo>
// v1.0

#ifndef STRUCTURES_ARRAY_LIST_H
#define STRUCTURES_ARRAY_LIST_H

#include <cstdint>  // std::size_t

namespace structures {

template <typename T>
//  Classe ArrayList, Lista em vetor
class ArrayList {
   public:
    // Construtor padrão
    ArrayList();
    // Construtor com parâmetro
    explicit ArrayList(std::size_t max_size);
    // Destrutor
    ~ArrayList();
    // Limpa a lista
    void clear();
    // Insere no fim
    void push_back(const T& data);
    // Insere no início
    void push_front(const T& data);
    // Insere na posição
    void insert(const T& data, std::size_t index);
    // Insere em ordem
    void insert_sorted(const T& data);
    // Retira na posição
    T pop(std::size_t index);
    // Retira no fim
    T pop_back();
    // Retira no início
    T pop_front();
    // Remove o dado
    void remove(const T& data);
    // Checa se a lista está cheia
    bool full() const;
    // Checa se a lista está vazia
    bool empty() const;
    // Checa se a lista contém um valor
    bool contains(const T& data) const;
    // Retorna o índice do dado encontrado
    std::size_t find(const T& data) const;
    // Retorna o tamanho
    std::size_t size() const;
    // Retorna o tamanho máximo
    std::size_t max_size() const;
    // Retorna na posição
    T& at(std::size_t index);
    // Sobrecarga do operador []
    T& operator[](std::size_t index);
    // Retorna na posição (constante)
    const T& at(std::size_t index) const;
    // Sobrecarga do operador [] (constante)
    const T& operator[](std::size_t index) const;

   private:
    T* contents;            // Dados
    std::size_t size_;      // Tamanho
    std::size_t max_size_;  // Tamanho máximo

    static const auto DEFAULT_MAX = 10u;
};

}  // namespace structures

/**
 * Constrói um objeto structures::ArrayList<T>.
 **/
template <typename T>
structures::ArrayList<T>::ArrayList() {
    max_size_ = DEFAULT_MAX;      // Inicializa o tamanho máximo
    contents = new T[max_size_];  // Inicializa a array
    size_ = 0;                    // Inicializa o tamanho
}

/**
 * Constrói um objeto structures::ArrayList<T>.
 *      Parâmetros:
 *          max_size (std::size_t): tamanho máximo.
 **/
template <typename T>
structures::ArrayList<T>::ArrayList(std::size_t max_size) {
    max_size_ = max_size;         // Inicializa o tamanho máximo
    contents = new T[max_size_];  // Inicializa a array
    size_ = 0;                    // Inicializa o tamanho
}

/**
 * Destrói o objeto structures::ArrayList<T>.
 **/
template <typename T>
structures::ArrayList<T>::~ArrayList() {
    delete[] contents;  // Apaga a array de conteúdo
}

/**
 * Limpa a lista.
 **/
template <typename T>
void structures::ArrayList<T>::clear() {
    size_ = 0;  // Redefine o tamanho
}

/**
 * Insere um dado no final da lista.
 *      Parâmetros:
 *          data (T): dado a ser inserido.
 **/
template <typename T>
void structures::ArrayList<T>::push_back(const T& data) {
    // Caso a lista não esteja cheia adiciona os dados no final

    if (!full()) {
        contents[size_++] = data;
    } else {
        throw std::out_of_range("Lista cheia");
    }
}

/**
 * Insere um dado no início da lista.
 *      Parâmetros:
 *          data (T): dado a ser inserido.
 **/
template <typename T>
void structures::ArrayList<T>::push_front(const T& data) {
    // Caso a lista não esteja cheia adiciona os dados no início

    if (!full()) {
        // Empurra todos os dados para frente
        for (std::size_t i = size(); i > 0; --i) {
            contents[i] = contents[i - 1];
        }

        // Incrementa o tamanho e adiciona o dado
        ++size_;
        contents[0] = data;
    } else {
        throw std::out_of_range("Lista cheia");
    }
}

/**
 * Insere um dado na posição do índice na lista.
 *      Parâmetros:
 *          data (T): Dado a ser inserido.
 *          index (std::size_t): Índice de inserção.
 **/
template <typename T>
void structures::ArrayList<T>::insert(const T& data, std::size_t index) {
    // Caso a lista não esteja cheia adiciona os dados na posição

    if (!full()) {
        if (index <= size() && index >= 0) {  // Verifica se a posição é válida
            // Empurra os dados depois da posição para frente
            for (std::size_t i = size(); i > index; --i) {
                contents[i] = contents[i - 1];
            }

            // Incrementa o tamanho e adiciona o dado
            ++size_;
            contents[index] = data;
        } else {
            throw std::out_of_range("Posição inválida");
        }

    } else {
        throw std::out_of_range("Lista cheia");
    }
}

/**
 * Insere um dado em ordem.
 *      Parâmetros:
 *          data (T): Dado a ser inserido.
 **/
template <typename T>
void structures::ArrayList<T>::insert_sorted(const T& data) {
    // Caso a lista não esteja cheia adiciona os dados em ordem

    if (!full()) {
        bool inserted = false;  // Variável para saber se está inserido

        /* O valor é comparado com cada valor da lista do fim ao início. Quando
           o valor da lista é menor do que o valor a ser inserido é feita a
           inserção.
        */

        // O loop itera do final ao início
        for (std::size_t i = size() - 1; i >= 0; --i) {
            // Checa se o valor é maior do que o dado na posição
            if (data > contents[i]) {
                if (i + 1 < static_cast<int>(size())) {
                    // Insere no meio
                    insert(data, i + 1);
                } else {
                    // Insere no fim
                    push_back(data);
                }

                inserted = true;  // Confirma que inseriu o valor
                break;
            }
        }

        // Caso o valor não tenha sido inserido antes ele é inserido no início
        if (!inserted) {
            push_front(data);
        }

    } else {
        throw std::out_of_range("Lista cheia");
    }
}

/**
 * Retorna e remove um dado no índice especificado.
 *      Parâmetros:
 *          index (std::size_t): Índice de remoção.
 *      Retorna o dado (T).
 **/
template <typename T>
T structures::ArrayList<T>::pop(std::size_t index) {
    // Retira o dado na posição caso a lista não esteja vazia

    if (!empty()) {
        // Verifica se a posição é válida
        if (index <= size() - 1 && index >= 0) {
            T element = contents[index];  // Obtém o dado

            // Empurra a lista após o dado para o início
            for (std::size_t i = index; i < size(); ++i) {
                contents[i] = contents[i + 1];
            }

            // Decrementa o tamanho e retorna o dado
            --size_;
            return element;
        } else {
            throw std::out_of_range("Posição inválida");
        }

    } else {
        throw std::out_of_range("Lista vazia");
    }
}

/**
 * Retorna e remove um dado no fim da lista.
 *      Retorna o dado (T).
 **/
template <typename T>
T structures::ArrayList<T>::pop_back() {
    // Retira o dado no fim caso a lista não esteja vazia

    if (!empty()) {
        T element = contents[size() - 1];  // Obtém o dado

        // Decrementa o tamanho e retorna o dado
        --size_;
        return element;
    } else {
        throw std::out_of_range("Lista vazia");
    }
}

/**
 * Retorna e remove um dado no início da lista.
 *      Retorna o dado (T).
 **/
template <typename T>
T structures::ArrayList<T>::pop_front() {
    if (!empty()) {
        T element = contents[0];  // Obtém o dado

        // Empurra a lista para o início
        for (std::size_t i = 0; i < size(); ++i) {
            contents[i] = contents[i + 1];
        }

        // Decrementa o tamanho e retorna o dado
        --size_;
        return element;
    } else {
        throw std::out_of_range("Lista vazia");
    }
}

/**
 * Remove o dado.
 *      Parâmetros:
 *          data (T): Dado a ser removido.
 **/
template <typename T>
void structures::ArrayList<T>::remove(const T& data) {
    // Remove o dado caso a lista não esteja vazia

    if (!empty()) {
        pop(find(data));  // Remove o dado encontrado na posição
    } else {
        throw std::out_of_range("Lista vazia");
    }
}

/**
 * Checa se a lista está cheia.
 *      Retorna verdadeiro caso a lista esteja cheia.
 **/
template <typename T>
bool structures::ArrayList<T>::full() const {
    // Verifica se a lista está cheia

    if (size() == max_size()) {
        return true;
    } else {
        return false;
    }
}

/**
 * Checa se a lista está vazia.
 *      Retorna verdadeiro caso a lista esteja vazia.
 **/
template <typename T>
bool structures::ArrayList<T>::empty() const {
    // Verifica se a lista está vazia

    if (size() == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * Checa se a lista contém o dado.
 *      Parâmetros:
 *          data (T): Dado a ser pesquisado.
 *      Retorna verdadeiro caso a lista tenha o dado.
 **/
template <typename T>
bool structures::ArrayList<T>::contains(const T& data) const {
    // Verifica se a lista contém o dado

    bool dataFound = false;  // Variável para armazenar o estado do dado

    if (!empty()) {                      // Caso a lista não esteja vazia
        std::size_t index = find(data);  // Procura o dado e obtém o seu índice

        // Caso o íncide corresponda ao dado isso significa que ele está na
        // lista e não ocorreu um erro
        if (index < max_size()) {
            if (contents[index] == data) {
                dataFound = true;
            }
        }
    } else {
        throw std::out_of_range("Lista vazia");
    }

    return dataFound;  // Retorna o estado da procura do dado
}

/**
 * Retorna o índice do dado.
 *      Parâmetros:
 *          data (T): Dado a ser pesquisado.
 *      Retorna o índice (std::size_t).
 **/
template <typename T>
std::size_t structures::ArrayList<T>::find(const T& data) const {
    // Procura pelo dado específico. Caso o dado não tenha sido encontrado
    // retorna a última posição

    std::size_t index = size();  // Armazena o índice

    // Itera pela lista até encontrar o dado
    for (std::size_t i = 0; i < size(); ++i) {
        if (contents[i] == data) {
            index = i;
            break;
        }
    }

    return index;
}

/**
 * Retorna o tamanho da lista (std::size_t).
 **/
template <typename T>
std::size_t structures::ArrayList<T>::size() const {
    // Retorna o tamanho
    return size_;
}

/**
 * Retorna o tamanho máximo da lista (std::size_t).
 **/
template <typename T>
std::size_t structures::ArrayList<T>::max_size() const {
    // Retorna o tamanho máximo
    return max_size_;
}

/**
 * Retorna o dado no índice (T) de forma segura.
 *      Parâmetros:
 *          index (std::size_t): Índice do dado.
 **/
template <typename T>
T& structures::ArrayList<T>::at(std::size_t index) {
    return const_cast<T&>(const_cast<const ArrayList*>(this)->at(index));
}

/**
 * Retorna o dado no índice (T).
 *      Parâmetros:
 *          index (std::size_t): Índice do dado.
 **/
template <typename T>
T& structures::ArrayList<T>::operator[](std::size_t index) {
    // Retorna o dado na posição
    return contents[index];
}

/**
 * Retorna o dado no índice (T) de forma segura e apenas para leitura.
 *      Parâmetros:
 *          index (std::size_t): Índice do dado.
 **/
template <typename T>
const T& structures::ArrayList<T>::at(std::size_t index) const {
    // Retorna o dado na posição caso a lista não esteja vazia e a posição seja
    // válida

    if (!empty()) {  // Verifica se a lista está vazia
        // Verifica se a posição é valida
        if (index <= size() - 1 && index >= 0) {
            return contents[index];  // Retorna o dado
        } else {
            throw std::out_of_range("Posição inválida");
        }

    } else {
        throw std::out_of_range("Lista vazia");
    }
}

/**
 * Retorna o dado no índice (T), apenas para leitura.
 *      Parâmetros:
 *          index (std::size_t): Índice do dado.
 **/
template <typename T>
const T& structures::ArrayList<T>::operator[](std::size_t index) const {
    // Retorna o dado na posição de forma constante (apenas leitura)
    return contents[index];
}

#endif
