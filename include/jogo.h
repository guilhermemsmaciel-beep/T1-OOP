#ifndef JOGO_H
#define JOGO_H

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "mapa.h"
#include "npc.h"
#include "pescador.h"

class Jogo {
private:
    static const unsigned int CUSTO_PESCA = 20;  // energia gasta por ida ao lago

    std::unique_ptr<Mapa> screen;
    std::unique_ptr<Npc> npc;
    std::vector<std::string> mapas;
    Pescador& player;
    std::string mensagem;   // aviso mostrado embaixo do mapa
    unsigned int dia;
    bool venceu;
    std::mt19937 rng;

    void printHUD() const;
    bool puxarPeixe(const Peixe& peixe);  // minigame de fisgada
    void mercado();
    void loja();
    void dormir();
    void falarComNpc();

public:
    explicit Jogo(Pescador& p);

    static void modoJogo();    // terminal lê tecla por tecla, sem eco
    static void modoTexto();   // terminal volta ao normal

    void game_preparation();   // carrega os mapas (lança ArquivoMapaException)
    void game_intro();         // nome do jogador, vara e iscas iniciais
    void run_fishing_game();   // pescaria no lago
    void run_game();           // loop principal andando pela vila
};

#endif
