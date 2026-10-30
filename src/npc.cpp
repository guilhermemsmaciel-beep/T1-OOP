#include "npc.h"

Npc::Npc(Vector2 pos, const std::string& nome)
    : Object(pos, 'N'), nome(nome), atual(0) {
    missoes = {
        {"Me traga 3 Lambaris para a minha sopa.", Dificuldade::Easy, 3, 25},
        {"Agora quero 2 Tilapias para o almoco de domingo.", Dificuldade::Medium, 2, 70},
    };
}

std::string Npc::getNome() const {
    return nome;
}

bool Npc::temMissao() const {
    return atual < missoes.size();
}

const Missao& Npc::missaoAtual() const {
    return missoes.at(atual);
}

void Npc::concluirMissao() {
    if (temMissao()) atual++;
}

unsigned int Npc::missoesConcluidas() const {
    return atual;
}

std::string Npc::fala() const {
    if (atual == 0)
        return "Ola, forasteiro! Sou o " + nome + ", pescador mais velho da vila.";
    if (temMissao())
        return "Obrigado pela ajuda! Tenho mais um pedido.";
    return "Dizem que o Dourado vive bem no fundo do lago. Com a Vara Lendaria "
           "e uma Isca Artificial voce consegue!";
}
