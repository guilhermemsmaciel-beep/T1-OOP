#include "isca.h"
#include <iostream>

Isca::Isca(const std::string& name, const std::string& desc, float valor,
           unsigned int bonus, unsigned int quantidade)
    : Item(name, desc, valor), bonus(bonus), quantidade(quantidade) {}

unsigned int Isca::getBonus() const {
    return bonus;
}

unsigned int Isca::getQuantidade() const {
    return quantidade;
}

void Isca::adicionar(unsigned int qtd) {
    quantidade += qtd;
}

bool Isca::usar() {
    if (quantidade == 0) return false;
    quantidade--;
    return true;
}

std::string Isca::categoria() const {
    return "Isca";
}

void Isca::itemDescription() const {
    Item::itemDescription();
    std::cout << "    Quantidade: " << quantidade
              << " | Bonus na fisgada: +" << bonus << std::endl;
}
