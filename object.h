#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include "raylib.h"

class Object {
private:
    Vector2 posicao;
    Vector2 size;
public:
    Object();
    Object(float x, float y, float sx, float sy) : 
           posicao({x, y}), size({sx, sy}) 
    {
        //
    }

    void setPos(Vector2 pos) {
        posicao = pos;
    }

    void setSize(Vector2 tam) {
        size = tam;
    }

    void Desenhar() {
        DrawRectangleV(posicao, size, WHITE);
        //std::cout << "teste6" << std::endl;
    }

    Vector2 size_of() {
        return size;
    }

    Vector2 posicao_of() {
        return posicao;
    }

};

#endif