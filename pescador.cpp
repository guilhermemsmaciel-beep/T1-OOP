#include "pescador.h"
#include <iostream>

Pescador::Pescador(float x, float y, float sx, float sy) : 
    vida_atual(100), velocidade(4.0), cor(PINK), carteira(0) 
{
    rec.x = x;
    rec.y = y;
    rec.width = sx;
    rec.height = sy;
}

bool Pescador::isAlive() {
    return vida_atual > 0;
}

Vector2 Pescador::getPosicao() {
    return {rec.x, rec.y};
}

void Pescador::setPosicao(Vector2 pos) {
    rec.x = pos.x;
    rec.y = pos.y;
}

void Pescador::setPosicaoX(float x) {
    rec.x = x;
}

void Pescador::setPosicaoY(float y) {
    rec.y = y;
}

void Pescador::LerEntrada(Mapa mapa) {
    // Métodos da Raylib para checar teclas pressionadas
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        if (mapa.Right() == nullptr) {
            if (rec.x < mapa.sizeOf().x - rec.width) rec.x += velocidade;
        }
        else rec.x += velocidade;
    }
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) {
        if (mapa.Left() == nullptr) {
            if (rec.x > 0.0) rec.x -= velocidade;
        }
        else rec.x -= velocidade;
    }
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) {
        if (mapa.Up() == nullptr) {
            if (rec.y > 0.0) rec.y -= velocidade;
        }
        else rec.y -= velocidade;
    }
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) {
        if (mapa.Down() == nullptr) {
            if (rec.y < mapa.sizeOf().y - rec.height) rec.y += velocidade;
        }       
        else rec.y += velocidade;
    } 
}

void Pescador::LerEntrada(Vector2 size) {
    // Métodos da Raylib para checar teclas pressionadas
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        if (rec.x < size.x - rec.width) 
            rec.x += velocidade;
        
    }
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) {
        if (rec.x > 0.0) 
            rec.x -= velocidade;
    }
}

EDirection Pescador::Atualizar(Mapa* obj) {
    // Lógica de colisão ou física entra aqui
    //std::cout << "Atualizar is working" << std::endl;
    if(vida_atual >= 0) {
        if (this->rec.x > obj->sizeOf().x) {
            return EDirection::Right;
        }

        if (this->rec.x < 0.0) {
            return EDirection::Left;
        }

        if (this->rec.y > obj->sizeOf().y) {
            return EDirection::Down;
        }

        if (this->rec.y < 0.0) {
            return EDirection::Up;
        }
    }
    return EDirection::None;
    
}

bool Pescador::Interagiu(Object obj) {
    float x1 = obj.posicao_of().x - (obj.size_of().x / 2);
    float y1 = obj.posicao_of().y - (obj.size_of().y / 2);
    float x2 = obj.posicao_of().x + (obj.size_of().x / 2);
    float y2 = obj.posicao_of().y + (obj.size_of().y / 2);
    if(rec.x >= x1 && rec.x <= x2) {
        if(rec.y >= y1 && rec.y <= y2) {
            return true;
        }
    }
    return false;
}


Texture2D Pescador::getTexture() {
    return texture;
}

Rectangle Pescador::getRec() {
    return rec;
}

void Pescador::Desenhar() {
    // Método da Raylib para desenhar formas na tela
   //DrawTexturePro(texture, sourceRec, rec, origin, 0.0f, cor);
    texture = LoadTexture("boat_raylib.png");
}

// void Pescador::Pescaria() {
//     while(!WindowShouldClose()) {
//         BeginDrawing();
//             ClearBackground(DARKBLUE);
//         EndDrawing();   
//     }
// }