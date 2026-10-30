#ifndef ISCA_H
#define ISCA_H

#include "item.h"

// HERANÇA 3: Isca é um Item
// Cada tentativa de fisgar gasta uma isca. A isca aumenta a zona verde
// do minigame de fisgada, deixando os peixes difíceis mais fáceis de puxar.
class Isca : public Item {
private:
    unsigned int bonus;       // quantas casas a zona verde ganha
    unsigned int quantidade;  // iscas iguais ficam empilhadas

public:
    Isca(const std::string& name, const std::string& desc, float valor,
         unsigned int bonus, unsigned int quantidade);

    unsigned int getBonus() const;
    unsigned int getQuantidade() const;
    void adicionar(unsigned int qtd);
    bool usar();  // gasta uma; false se já acabou

    std::string categoria() const override;
    void itemDescription() const override;
};

#endif
