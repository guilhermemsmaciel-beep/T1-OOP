#ifndef ROD_H
#define ROD_H

#include "item.h"

#define SIZE_X 20.0
#define SIZE_Y 20.0

class Rod : public Item {
private:
    float depth_limit;
    float distance_limit;
public:
    Rod() : depth_limit(10), distance_limit(10) {
        nome = "Regular Fishing Rod";
        descricao = "Just a regular fishing rod, great for catching small fish";
    }

    // void itemDescription() {
    //     Item::itemDescription();
    // }

    // std::string getNome() {
    //     return Item::getNome();
    // }

    // void LerEntrada(Vector2 player_pos) {
    //     pos = player_pos;
    //     depth_limit = pos.y + 200;
    //     distance_limit = pos.x + 100;
    //     if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    //         if (pos.x <= distance_limit) 
    //             pos.x += 2.0;
    //     }
    //     if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) {
    //         if (pos.x >= player_pos.x)
    //             pos.x -= 2.0;
    //     }
    //     if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) {
    //         if (pos.y >= player_pos.y)
    //             pos.y -= 2.0;
    //     }
    //     if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) {
    //         if (pos.y <= depth_limit)
    //             pos.y += 2.0;
    //     } 
    //     if (IsKeyDown(KEY_F))
    //         return;
    // }
    // void Desenhar(float width, float height) {
    //     pos.x += width;
    //     pos.y += height;
    //     DrawRectangleV(pos, size, GREEN);
    // }

};


#endif