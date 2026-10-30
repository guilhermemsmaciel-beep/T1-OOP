#ifndef JOGO_H
#define JOGO_H

#include <memory>
#include <string>
#include <vector>

#include "mapa.h"
#include "pescador.h"

class Jogo {
private:
    std::unique_ptr<Mapa> screen;
    std::vector<std::string> mapas;
    Pescador& player;
    std::string mensagem;   // aviso mostrado embaixo do mapa
    bool venceu;

    void printHUD() const;
    void mercado();
    void loja();

public:
    explicit Jogo(Pescador& p);

    static void modoJogo();    // terminal lê tecla por tecla, sem eco
    static void modoTexto();   // terminal volta ao normal

    void game_preparation();   // carrega os mapas (lança ArquivoMapaException)
    void game_intro();         // nome do jogador e vara inicial
    void run_fishing_game();   // minigame de pesca no lago
    void run_game();           // loop principal andando pela vila
};

#endif
