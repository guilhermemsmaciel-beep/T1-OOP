#ifndef JOGADOR_H
#define JOGADOR_H

#include <vector>

#include "raylib.h"
#include "mapa.h"
#include "object.h"
#include "peixe.h"
#include "item.h"
#include "edirection.h"

class Pescador {
private:
    std::string nome;
    unsigned int vida_atual;
    Texture2D texture;
    Rectangle rec;
    float velocidade;
    Color cor;
    std::vector<Peixe> estoque;
    std::vector<Item> inventario;
    float carteira;


public:
    Pescador(float x, float y, float sx, float sy);
    bool isAlive();
    Vector2 getPosicao();
    void setPosicao(Vector2 pos);
    void setPosicaoX(float x);
    void setPosicaoY(float y);
    Texture2D getTexture();
    Rectangle getRec();
    void LerEntrada(Mapa map);
    void LerEntrada(Vector2 size);
    EDirection Atualizar(Mapa* obj);
    bool Interagiu(Object obj);
    void Desenhar();

};

#endif