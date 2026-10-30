#include "fisgada.h"
#include <algorithm>

Fisgada::Fisgada(Dificuldade dif, unsigned int bonus_isca, std::mt19937& rng)
    : marcador(0), direcao(1), passadas(0) {
    unsigned int base = 8;
    switch (dif) {
        case Dificuldade::Easy:   base = 8; intervalo_ms = 90; break;
        case Dificuldade::Medium: base = 5; intervalo_ms = 70; break;
        case Dificuldade::Hard:   base = 3; intervalo_ms = 50; break;
    }
    zona_tamanho = std::min(base + bonus_isca, LARGURA - 2);

    // a zona verde nunca começa grudada no início, onde o marcador nasce
    std::uniform_int_distribution<unsigned int> pos(4, LARGURA - zona_tamanho);
    zona_inicio = std::min(pos(rng), LARGURA - zona_tamanho);
}

void Fisgada::avancar() {
    if (direcao > 0 && marcador >= LARGURA - 1) {
        direcao = -1;
    } else if (direcao < 0 && marcador == 0) {
        direcao = 1;
        passadas++;
    }
    marcador += direcao;
}

bool Fisgada::dentroDaZona() const {
    return marcador >= zona_inicio && marcador < zona_inicio + zona_tamanho;
}

bool Fisgada::esgotou() const {
    return passadas >= MAX_PASSADAS;
}

unsigned int Fisgada::getIntervaloMs() const {
    return intervalo_ms;
}

unsigned int Fisgada::getZonaTamanho() const {
    return zona_tamanho;
}

std::string Fisgada::desenhar() const {
    const std::string verde = "\033[42m";
    const std::string normal = "\033[0m";

    std::string barra = "[";
    for (unsigned int i = 0; i < LARGURA; i++) {
        bool naZona = (i >= zona_inicio && i < zona_inicio + zona_tamanho);
        char c = (i == marcador) ? '|' : (naZona ? '=' : '-');
        if (naZona) barra += verde + c + normal;
        else barra += c;
    }
    barra += "]";
    return barra;
}
