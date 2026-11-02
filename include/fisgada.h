#ifndef FISGADA_H
#define FISGADA_H

#include <random>
#include <string>

#include "dificuldade.h"

// Minigame de fisgada: um marcador corre de um lado para o outro numa barra
// e o jogador precisa apertar ESPAÇO quando ele estiver na zona verde.
// Peixes difíceis têm zona menor e marcador mais rápido; iscas aumentam a zona.
class Fisgada {
private:
    static const unsigned int LARGURA = 30;
    static const unsigned int MAX_PASSADAS = 3;  // idas e voltas antes do peixe fugir

    unsigned int zona_inicio;
    unsigned int zona_tamanho;
    unsigned int marcador;
    int direcao;
    unsigned int passadas;
    unsigned int intervalo_ms;

public:
    Fisgada(Dificuldade dif, unsigned int bonus_isca, std::mt19937& rng);

    void avancar();             // move o marcador uma casa
    bool dentroDaZona() const;  // o marcador está na zona verde?
    bool esgotou() const;       // o peixe cansou de esperar e fugiu
    unsigned int getIntervaloMs() const;
    unsigned int getZonaTamanho() const;
    std::string desenhar() const;
};

#endif
