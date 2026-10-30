#ifndef PEIXE_H
#define PEIXE_H

#include <string>
#include "object.h"
#include "validade.h"
#include "dificuldade.h"

// HERANÇA 2: Peixe é um Object (tem posição no mapa e um símbolo)
class Peixe : public Object {
private:
    std::string nome;
    float valor;
    Validade validade;
    Dificuldade dificuldade;
    int direcao;  // +1 nadando para a direita, -1 para a esquerda

public:
    Peixe(Vector2 pos, Dificuldade dif);

    // Move o peixe na horizontal, dando a volta nas bordas do lago
    void Swim(unsigned int x_min, unsigned int x_max);

    // Cada ida ao lago envelhece os peixes guardados: Fresco -> Passado -> Podre
    void envelhecer();

    std::string getNome() const;
    float getValor() const;
    float getPrecoVenda() const;
    Validade getValidade() const;
    Dificuldade getDificuldade() const;

    char getSimbolo() const override;
    void printFish() const;
};

std::string validadeToString(Validade v);
std::string dificuldadeToString(Dificuldade d);

#endif
