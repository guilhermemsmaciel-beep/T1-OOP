#ifndef NPC_H
#define NPC_H

#include <string>
#include "gtools.h"

class Npc {
private:
  std::string text;
  Vector2 posicao;

public:
  Npc() : posicao({0, 0});
  void setText(std::string texto);
  std::string getText();
}

#endif
