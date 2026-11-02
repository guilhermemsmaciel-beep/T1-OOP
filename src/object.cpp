#include "object.h"

Object::Object() : posicao(), simbolo('O') {}

Object::Object(Vector2 pos, char simbolo) : posicao(pos), simbolo(simbolo) {}

Vector2 Object::getPos() const {
    return posicao;
}

void Object::setPos(Vector2 pos) {
    posicao = pos;
}

char Object::getSimbolo() const {
    return simbolo;
}
