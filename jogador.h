#ifndef JOGADOR_H
#define JOGADOR_H

#include <string>
#include <vector>
#include "gtools.h"

class Jogador{
private:
  std::string nome;
  unsigned int vida_atual;
  std::vector<Peixe> estoque;
  Vector2 posicao;
  Vector2 size;
  float dinheiro;
public:
  Jogador() : posicao({0, 0}), size({0, 0}), vida_atual(0), dinheiro(0) {}
  void setNome(std::string name);
  void setVida(unsigned int vida);
  void setEstoque(Peixe peixe);
  void setPosicao(Vector2 pos);
  void setSize(Vector tam);
  void setDinheiro(float money);

};

#endif
