#ifndef PEIXE_H
#define PEIXE_H

#include <string>

enum class EValidade {
  Fresco,
  Passado,
  Podre
};

class Peixe {
private:
  std::string nome;
  Vector2 posicao;
  Vector2 size;
  std::string descricao;
  EValidade validade;
  float preco;
public:
  Peixe() : posicao({0, 0}), size({0, 0}), validade(EValidade::Fresco), preco(5) {};
  void setNome(std::string name);
  void setPosicao(Vector2 pos);
  void setSize(Vector2 tam);
  void setDescricao(std::string text);
  void setValidade();
  void setPreco();
}

#endif
