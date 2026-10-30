#include <csignal>
#include <cstdlib>
#include <iostream>

#include "excecoes.h"
#include "jogo.h"
#include "pescador.h"

// Se o jogador apertar Ctrl+C, devolve o terminal ao normal antes de sair
static void aoInterromper(int) {
    Jogo::modoTexto();
    std::_Exit(130);
}

int main() {
    std::signal(SIGINT, aoInterromper);

    Pescador player;
    Jogo fishz(player);

    try {
        fishz.game_preparation();
    } catch (const ArquivoMapaException& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Rode o jogo a partir da pasta raiz do projeto (make run)." << std::endl;
        return 1;
    }

    try {
        Jogo::modoJogo();
        fishz.game_intro();
        fishz.run_game();
    } catch (const JogoException& e) {
        Jogo::modoTexto();  // garante que o terminal volta ao normal
        std::cerr << "Erro no jogo: " << e.what() << std::endl;
        return 1;
    }

    Jogo::modoTexto();
    return 0;
}
