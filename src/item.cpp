#include "item.h"
#include <iostream>

Item::Item() : nome("Item"), descricao(""), valor(0) {}

Item::Item(const std::string& name, const std::string& desc, float valor)
    : nome(name), descricao(desc), valor(valor) {}

std::string Item::getNome() const {
    return nome;
}

float Item::getValor() const {
    return valor;
}

void Item::itemDescription() const {
    std::cout << "  " << nome << " ($" << valor << ")" << std::endl;
    std::cout << "    " << descricao << std::endl;
}
