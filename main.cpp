#include <iostream>
#include <fstream>
#include <iostream>
#include "BinarySearchTree.hpp"

int main() {
    setlocale(LC_ALL, "ru");
    Dictionary<std::string> vocabulary;
    std::string filename = "file.txt";
    std::ifstream stream(filename);
    try {
        if (!stream.is_open()) {
            throw std::runtime_error("Не удалось открыть файл");
        }
    }
    catch (const std::runtime_error& error) {
        std::cerr << error.what() << "\n";
        return 0;
    }
    while (!stream.eof()) {
        std::string word;
        getline(stream, word);
        if (word != "") {
            vocabulary.INSERTWORD(word);
        }
        std::string translation;
        getline(stream, translation);
        getline(stream, translation);
        while (translation != "")
        {
            if (translation != "Нет переводов") {
                vocabulary.ADDTRANSLATION(word, translation);
            }
            getline(stream, translation);
        }
    }
    std::string operation;
    std::cout << "Выберите номер операции:\n1 - записать новое слово\n2 - записать новый перевод\n3 - показать перевод слова\n4 - удалить слово\n5 - показать все слова в словаре\n6 - завершить программу\n";
    std::cin >> operation;
    while (operation != "6") {
        if (operation == "1") {
            std::string newWord;
            std::cout << "Введите слово, которое хотите добавить в словарь:\n";
            std::cin >> newWord;
            vocabulary.INSERTWORD(newWord);
        }
        else if (operation == "2") {
            std::string word;
            std::cout << "Введите слово, для которого хотите добавить перевод:\n";
            std::cin >> word;
            std::string translation;
            std::cout << "Введите новый перевод слвоа:\n";
            std::cin >> translation;
            vocabulary.ADDTRANSLATION(word, translation);
        }
        else if (operation == "3") {
            std::string word;
            std::cout << "Введите слово, которое хотите перевести:\n";
            std::cin >> word;
            vocabulary.TRANSLATEWORD(word);
        }
        else if (operation == "4") {
            std::string word;
            std::cout << "Введите слово, которое хотите удалить:\n";
            std::cin >> word;
            vocabulary.DELETEWORD(word);
        }
        else if (operation == "5") {
            vocabulary.VIEWALLWORDS();
        }
        else {
            std::cout << "Неверная операция, попробуйте снова!\n";
        }
        std::cout << "Введите номер операции:\n";
        std::cin >> operation;
    }
    vocabulary.reloadTheDictionary(filename);
    return 0;
}