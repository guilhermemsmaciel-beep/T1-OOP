#ifndef MAPA_H
#define MAPA_H

#include <iostream>
#include "raylib.h"
#include "object.h"

class Mapa {
private: 
    //Vector2 posicao;
    std::string address;
    Vector2 size;
    Mapa* left;
    Mapa* right;
    Mapa* up;
    Mapa* down;
    //std::vector<Object> objetos;
    Color cor;
public:
    Mapa() : 
        size({0 ,0}), cor(BLUE), 
        left(nullptr), right(nullptr), 
        up(nullptr), down(nullptr) 
    {
        // Left blank on purpose 
    }

    Mapa(std::string address, float sx, float sy, Color c) : 
        address(address), 
        size({sx, sy}), cor(c), 
        left(nullptr), right(nullptr), 
        up(nullptr), down(nullptr) 
    {
        // Left blank on purpose
    }

    // Mapa(float sx, float sy, Color c, Object obj) : 
    //     size({sx, sy}), cor(c), 
    //     left(nullptr), right(nullptr), 
    //     up(nullptr), down(nullptr) 
    // {
    //     objetos.push_back(obj);
    // }

    std::string Address() {
        return address;
    }

    Vector2 sizeOf() {
        return this->size;
    }

    Mapa* Left() {
        return left;
    }

    Mapa* Right() {
        return right;
    }

    // void setRight() {
    //     std::cout << "initiating function" << std::endl; 
    //     this->right = new Mapa("teste", size.x, size.y, BLUE);
    //     right->left = this;
    //     std::cout << "it worked!" << std::endl;
    // }

    void setDirections(Mapa* left, Mapa* right, Mapa* up, Mapa* down) {
        this->left = left;
        this->right = right;
        this->up = up;
        this->down = down;
    }

    // void setDirections() {
    //     left = new Mapa(size.x, size.y, BLUE);
    //     right = new Mapa(size.x, size.y, BLUE);
    //     up = new Mapa(size.x, size.y, BLUE);
    //     down = new Mapa(size.x, size.y, BLUE);

    //     left->right = this;

    //     right->left = this;

    //     up->down = this;

    //     down->up = this;
    // }

    Mapa* Up() {
        return up;
    }

    Mapa* Down() {
        return down;
    }

    // void addObject(Object obj) {
    //     objetos.push_back(obj);
    // }

    Color getCor() {
        return cor;
    }

    // auto mapBegin() {
    //     return this->objetos.begin();
    // }

    // auto mapEnd() {
    //     return this->objetos.end();
    // }

    // void Desenhar() {
    //     // Carrega do arquivo o mapa e desenha ele de acordo com a posicao do jogador
    //     // Quando jogador sai por um canto do mapa, spawna ele na posicao inversa no mapa correspondente
    //     objetos[0].Desenhar();
    // }

    
};

#endif