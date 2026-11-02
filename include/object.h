#ifndef OBJECT_H
#define OBJECT_H

#include "gtools.h"

// Qualquer coisa posicionada no mapa (lago, mercado, loja, peixes...)
class Object {
protected:
    Vector2 posicao;
    char simbolo;

public:
    Object();
    Object(Vector2 pos, char simbolo = 'O');
    virtual ~Object() = default;

    Vector2 getPos() const;
    void setPos(Vector2 pos);

    // virtual: Peixe muda o símbolo conforme a direção em que nada
    virtual char getSimbolo() const;
};

#endif
