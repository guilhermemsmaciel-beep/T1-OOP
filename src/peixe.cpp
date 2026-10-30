#include "peixe.h"
#include <iostream>

Peixe::Peixe(Vector2 pos, Dificuldade dif)
    : Object(pos, '>'), validade(Validade::Fresco), dificuldade(dif), direcao(1) {
    switch (dif) {
        case Dificuldade::Easy:
            nome = "Lambari";
            valor = 5;
            break;
        case Dificuldade::Medium:
            nome = "Tilapia";
            valor = 15;
            break;
        case Dificuldade::Hard:
            nome = "Dourado";
            valor = 50;
            break;
    }
    // metade dos peixes começa nadando para a esquerda
    if (pos.x % 2 == 0) direcao = -1;
}

void Peixe::Swim(unsigned int x_min, unsigned int x_max) {
    // vira ao bater na borda do lago
    if (direcao > 0 && posicao.x >= x_max) direcao = -1;
    if (direcao < 0 && posicao.x <= x_min) direcao = 1;
    posicao.x += direcao;
}

void Peixe::envelhecer() {
    if (validade == Validade::Fresco)
        validade = Validade::Passado;
    else if (validade == Validade::Passado)
        validade = Validade::Podre;
}

std::string Peixe::getNome() const {
    return nome;
}

float Peixe::getValor() const {
    return valor;
}

float Peixe::getPrecoVenda() const {
    switch (validade) {
        case Validade::Fresco:  return valor;
        case Validade::Passado: return valor / 2;
        case Validade::Podre:   return 0;
    }
    return 0;
}

Validade Peixe::getValidade() const {
    return validade;
}

Dificuldade Peixe::getDificuldade() const {
    return dificuldade;
}

char Peixe::getSimbolo() const {
    return direcao > 0 ? '>' : '<';
}

void Peixe::printFish() const {
    std::cout << "  " << nome
              << " [" << dificuldadeToString(dificuldade) << "] "
              << validadeToString(validade)
              << " - vale $" << getPrecoVenda() << std::endl;
}

std::string validadeToString(Validade v) {
    switch (v) {
        case Validade::Fresco:  return "Fresco";
        case Validade::Passado: return "Passado";
        case Validade::Podre:   return "Podre";
    }
    return "?";
}

std::string dificuldadeToString(Dificuldade d) {
    switch (d) {
        case Dificuldade::Easy:   return "Facil";
        case Dificuldade::Medium: return "Medio";
        case Dificuldade::Hard:   return "Dificil";
    }
    return "?";
}
