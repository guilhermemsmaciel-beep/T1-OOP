#ifndef JOGADOR_H
#define JOGADOR_H

#include <string>
#include <vector>
#include "gtools.h"
#include "peixe.h"
#include "item.h"
#include "fishingrod"

class Jogador{
private:
  std::string nome;
  unsigned int vida_atual;
  Vector2 posicao;
  std::vector<Peixe> estoque;
  std::vector<Item*> inventario;
  Rod* equipped_rod;
  float dinheiro;
public:
  Jogador();
  Jogador(std::string name);
  std::string getNome();
  void addRod(Rod* rod);
  void printInventario();
  bool isAlive();
  unsigned int getPosX();
  unsigned int getPosY();
  Vector2 getPos();
  void atualizarPos(char comando, std::vector<std::vector<char>> mapa);
  void atualizarPos(char comando, std::vector<std::vector<char>> mapa, char rpos);
  void setPos(Vector2 pos);
  void setNome(std::string name);
};

#endif
