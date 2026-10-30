#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

class Item {
protected:
    std::string nome;
    std::string descricao;
    unsigned int valor;
public:
    Item() : valor(5) {}

    Item(std::string name, std::string desc) : valor(5), nome(name), descricao(desc) {}

    std::string getNome() {
        return nome;
    }

    void itemDescription() {
        std::cout << nome << std::endl;
        std::cout << descricao << std::endl;
    }
};

#endif