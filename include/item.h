#ifndef ITEM_H
#define ITEM_H

#include <string>

// Classe base de tudo que pode ir para o inventário do pescador
class Item {
protected:
    std::string nome;
    std::string descricao;
    float valor;

public:
    Item();
    Item(const std::string& name, const std::string& desc, float valor = 0);
    virtual ~Item() = default;

    std::string getNome() const;
    float getValor() const;

    // virtual: as classes filhas podem mostrar informações extras
    virtual void itemDescription() const;
};

#endif
