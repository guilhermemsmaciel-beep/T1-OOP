#ifndef GTOOLS_H
#define GTOOLS_H

#include <iostream>

// Ferramentas gerais do jogo (posição no grid e tela)

// Limpa o terminal com um código ANSI (mais rápido que system("clear"),
// o que evita a tela piscando no minigame de fisgada)
inline void limparTela() {
    std::cout << "\033[2J\033[H";
}

struct Vector2 {
    unsigned int x;
    unsigned int y;

    Vector2() : x(0), y(0) {}
    Vector2(unsigned int px, unsigned int py) : x(px), y(py) {}

    bool operator==(const Vector2& comp) const {
        return x == comp.x && y == comp.y;
    }

    bool operator!=(const Vector2& comp) const {
        return !(*this == comp);
    }
};

#endif
