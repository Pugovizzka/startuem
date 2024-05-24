#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include "wordTranslation.h"

template <class T>
class Dictionary
{
public:

    Dictionary();
    Dictionary(const Dictionary<T>& scr) = delete;
    Dictionary(Dictionary<T>&& scr) noexcept;
    virtual ~Dictionary();
    void INSERTWORD(const T& word);
    void ADDTRANSLATION(const T& word, const T& translation);
    void DELETEWORD(const T& word);
    void TRANSLATEWORD(const T& word);
    void VIEWALLWORDS();
    void reloadTheDictionary(const std::string& filename);
    int getNumberOfWords() const;

private:
    struct Node {
        T data_;
        Node* p_;
        Node* left_;
        Node* right_;
        List<T> translations_;

        Node(T data, Node* p = nullptr, Node* left = nullptr, Node* right = nullptr) :
            data_(data),
            p_(p),
            left_(left),
            right_(right)
        { }
    };
    Node* root_;
    void deleteSubtree(Node* node);
    Node* SearchNode(const T& key) const;
    void iterativeWalk(const Node* node, int& size);
    void printInFile(Node* node, std::ofstream& stream);
    int getCount(const Node* node) const;
};

template <class T>
Dictionary<T>::Dictionary() :
    root_(nullptr)
{}

template <class T>
Dictionary<T>::Dictionary(Dictionary<T>&& scr) noexcept {
    root_ = new Node(scr.root_);
    deleteSubtree(scr);
}

template <class T>
Dictionary<T>::~Dictionary() {
    deleteSubtree(root_);
    delete root_;
}

template <class T>
void Dictionary<T>::deleteSubtree(Node* node) {
    if (root_ == nullptr) {
        return;
    }
    if (node->right_ != nullptr) {
        deleteSubtree(node->right_);
    }
    if (node->left_ != nullptr) {
        deleteSubtree(node->left_);
    }
}

template <class T>
void Dictionary<T>::INSERTWORD(const T& word) {
    std::string info = word;
    for (int i = 0; i < info.size(); i++) {
        if ((info[i] - 'a' < 0) || (info[i] - 'a' > 25)) {
            std::cout << "Допущена ошибка при вводе слова, попробуйте снова!\n";
            return;
        }
    }
    if (root_ == nullptr) {
        root_ = new Node(word);
    }
    else {
        Node* temp = root_;
        Node* tempnext = temp;
        while (tempnext != nullptr) {
            temp = tempnext;
            if (temp->data_ == word) {
                std::cout << "Этот перевод уже существует, попробуйте снова!\n";
                return;
            }
            if (temp->data_ > word) {
                tempnext = temp->left_;
            }
            else if (temp->data_ < word) {
                tempnext = temp->right_;
            }
        }
        if (temp->data_ > word) {
            temp->left_ = new Node(word, temp);
        }
        if (temp->data_ < word) {
            temp->right_ = new Node(word, temp);
        }
    }
}

template <class T>
void Dictionary<T>::DELETEWORD(const T& word) {
    if (this->getNumberOfWords() == 0)
    {
        std::cout << "Словарь сейчас пуст!\n";
        return;
    }
    Node* temp = SearchNode(word);
    if (temp == nullptr) {
        std::cout << "Не удалось найти это слово, попробуйте снова!\n";
        return;
    }
    Node* tempprev = temp->p_;
    if ((temp->right_ == nullptr) && (temp->left_ == nullptr)) {
        if (tempprev->data_ < temp->data_) {
            tempprev->right_ = nullptr;
        }
        if (tempprev->data_ > temp->data_) {
            tempprev->left_ = nullptr;
        }
        delete temp;
    }
    else if (((temp->right_ != nullptr) || (temp->left_ != nullptr)) && !((temp->right_ != nullptr) && (temp->left_ != nullptr)))
    {
        if (temp->right_ == nullptr) {
            if (tempprev->data_ < tempprev->data_) {
                tempprev->left_ = temp->left_;
                delete temp;
            }
            else if (tempprev->data_ > tempprev->data_) {
                tempprev->right_ = temp->right_;
                delete temp;
            }
        }
        if (temp->left_ == nullptr) {
            if (tempprev->data_ < temp->data_) {
                tempprev->right_ = temp->right_;
                delete temp;
            }
            else if (tempprev->data_ > temp->data_) {
                tempprev->left_ = temp->right_;
                delete temp;
            }
        }
    }
    else if ((temp->right_ != nullptr) && (temp->left_ != nullptr)) {
        tempprev = temp;
        temp = temp->right_;
        while ((temp->right_ != nullptr) && (temp->left_ != nullptr)) {
            temp = temp->left_;
        }
        if (tempprev->right_ != temp) {
            tempprev->data_ = temp->data_;
            Node* prev = temp->p_;
            if (temp->data_ == prev->left_->data_) {
                prev->left_ = nullptr;
            }
            if (temp->data_ == prev->right_->data_) {
                prev->left_ = nullptr;
            }
        }
        else {
            tempprev->data_ = temp->data_;
            tempprev->right_ = nullptr;
        }
        delete temp;
    }
    else {
        std::cout << "Не удалось найти это слово, попробуйте снова!\n";
        return;
    }
}

template<class T>
typename Dictionary<T>::Node* Dictionary<T>::SearchNode(const T& key) const {
    Node* temp = root_;
    if (temp == nullptr) {
        return temp;
    }
    while ((temp != nullptr)) {
        if (temp->data_ == key) {
            return temp;
        }
        else if (temp->data_ > key) {
            temp = temp->left_;
        }
        else if (temp->data_ < key) {
            temp = temp->right_;
        }
    }
    if (temp == root_) {
        return temp;
    }
    else {
        temp = nullptr;
        return temp;
    }
}

template<class T>
void Dictionary<T>::ADDTRANSLATION(const T& word, const T& translation) {
    Node* temp = SearchNode(word);
    if (temp == nullptr) {
        this->INSERTWORD(word);
    }
    temp = SearchNode(word);
    if (temp == nullptr) {
        return;
    }
    temp->translations_.insert(translation);
}

template<class T>
void Dictionary<T>::TRANSLATEWORD(const T& word) {
    if (this->getNumberOfWords() == 0)
    {
        std::cout << "Словарь сейчас пуст!\n";
        return;
    }
    Node* temp = SearchNode(word);
    if (temp == nullptr) {
        std::cout << "Не удалось найти это слово, попробуйте снова!\n";
        return;
    }
    std::cout << "Слово: " << temp->data_ << "\n";
    std::cout << "Перевод: ";
    temp->translations_.print();
}

template<class T>
void Dictionary<T>::VIEWALLWORDS() {
    if (root_ == nullptr) {
        std::cout << "Словарь сейчас пуст!";
        return;
    }
    std::cout << "Словарь содержит следующие слова:\n\n";
    int size = 0;
    iterativeWalk(root_, size);
    std::cout << "\nОбщее количество слов в словаре: " << size << "\n\n";
}

template<class T>
void Dictionary<T>::iterativeWalk(const Dictionary::Node* node, int& size) {
    if (node != nullptr)
    {
        iterativeWalk(node->left_, size);
        std::cout << node->data_ << "\n";
        size++;
        iterativeWalk(node->right_, size);
    }
}

template<class T>
void Dictionary<T>::reloadTheDictionary(const std::string& filename) {
    std::ofstream stream(filename);
    Node* node = root_;
    printInFile(node, stream);
}

template<class T>
void Dictionary<T>::printInFile(Node* node, std::ofstream& stream) {
    if (node != nullptr)
    {
        printInFile(node->left_, stream);
        stream << node->data_ << "\n\n";
        node->translations_.printFile(stream);
        stream << "\n";
        printInFile(node->right_, stream);
    }
}

template <class T>
int Dictionary<T>::getNumberOfWords() const {
    return getCount(this->root_);
}

template <class T>
int Dictionary<T>::getCount(const Node* node) const {
    if (node == nullptr)
        return 0;
    return (1 + getCount(node->left_) +
        getCount(node->right_));
}
