#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>

#include "object.h"
#include "dificuldade.h"

// Um pedido do NPC: "me traga X peixes do tipo Y"
struct Missao {
    std::string pedido;
    Dificuldade tipo;
    unsigned int quantidade;
    float recompensa;
};

// HERANÇA 4: Npc é um Object (fica parado num ponto do mapa)
// O NPC da vila passa missões, uma de cada vez, e paga por elas.
class Npc : public Object {
private:
    std::string nome;
    std::vector<Missao> missoes;
    unsigned int atual;  // índice da missão em andamento

public:
    Npc(Vector2 pos, const std::string& nome);

    std::string getNome() const;
    bool temMissao() const;
    const Missao& missaoAtual() const;  // só chame se temMissao()
    void concluirMissao();
    unsigned int missoesConcluidas() const;
    std::string fala() const;
};

#endif
