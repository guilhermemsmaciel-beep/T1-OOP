#ifndef PEIXE_H
#define PEIXE_H

#include <string>

enum class Validade {
    Fresco,
    Passado,
    Podre
};

class Peixe {
private:
    std::string nome;
    Texture2D textura;
    Rectangle rec;
    std::string descricao;
    Validade validade;
    float valor;
    bool bigger = false;
    // T dificuldade;
public:
    Peixe() : valor(0) {}
    Peixe(float x, float y, float sx, float sy) : valor(0) {
        rec.x = x;
        rec.y = y;
        rec.width = sx;
        rec.height = sy;
        textura = LoadTexture("wabbit_alpha.png");
    }
    void Swim() {
        if(rec.x > 800) {
            bigger = true;
        }
        if(rec.x < 0) {
            bigger = false;
        }
        if(bigger) rec.x -= 2.0;
        else rec.x += 2.0;
    }

    void Desenhar() {
        DrawRectangle(rec.x, rec.y, rec.width, rec.height, GREEN);
        //DrawTexture(textura, rec.x, rec.y, WHITE);
    };
};


#endif