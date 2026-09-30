#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include "gtools.h"

struct Tile {
  Vector2 size;
  Vector2 position;

  Tile() : size({0, 0}), position({0, 0})
  void setSize(Vector2 tam);
  void setPosition(Vector2 pos);
}

class Mapa {
private:
  Vector2 top_left;
  Vector2 bottom_right;
  std::vector<Tile> map;
  std::vector<Npc> Npcs;

public:
  Mapa() : top_lef({0, 0}), bottom_right({0, 0}) {}
  
};

#endif
