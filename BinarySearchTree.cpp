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

