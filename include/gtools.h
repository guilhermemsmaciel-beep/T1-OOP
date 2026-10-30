#ifndef GTOOLS_H
#define GTOOLS_H

// Ferramentas gerais do jogo (posição no grid)

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
