#include "jogo.h"
#include "excecoes.h"

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>

// O jogo roda com o terminal em modo "cbreak" (sem Enter e sem eco),
// assim cada tecla chega na hora. O modo normal só volta para digitar o nome.
void Jogo::modoJogo() {
    system("stty -icanon -echo");
}

void Jogo::modoTexto() {
    system("stty sane");
}

// LAMBDA: lê uma única tecla
static auto lerTecla = []() -> char {
    return static_cast<char>(std::getchar());
};

static void esperarTecla() {
    std::cout << "\n(aperte qualquer tecla para continuar)" << std::endl;
    lerTecla();
}

Jogo::Jogo(Pescador& p) : screen(nullptr), player(p), venceu(false) {}

void Jogo::game_preparation() {
    screen = std::make_unique<Mapa>(player);
    mapas.push_back("assets/mapas/vila.txt");
    mapas.push_back("assets/mapas/lago.txt");
    screen->loadMapas(mapas);
}

void Jogo::game_intro() {
    system("clear");
    std::string nome;
    char answer = 'n';

    std::cout << "Bem-vindo ao Fishz!\n" << std::endl;
    while (answer != 's' && answer != 'S') {
        modoTexto();
        std::cout << "Qual o nome do seu personagem? ";
        std::getline(std::cin, nome);
        modoJogo();
        if (nome.empty()) continue;
        std::cout << "Tem certeza que quer se chamar " << nome << "? (s/n) ";
        answer = lerTecla();
        std::cout << answer << std::endl;
    }
    player.setNome(nome);

    std::cout << "\nCerto, " << player.getNome()
              << ". Primeiro, a sua ferramenta mais importante: a vara de pesca!" << std::endl;
    std::cout << "Toma essa aqui. Nao e a mais bonita, mas pega os peixes pequenos.\n" << std::endl;

    player.addRod(std::make_unique<Rod>());
    player.getRod()->itemDescription();

    std::cout << "\nSeu objetivo: pescar um DOURADO, o peixe que vive no fundo do lago." << std::endl;
    std::cout << "Venda o que pescar no mercado ($) e compre varas melhores na loja (L)." << std::endl;
    std::cout << "Cuidado: a cada ida ao lago os peixes guardados envelhecem!" << std::endl;
    esperarTecla();
}

void Jogo::printHUD() const {
    const Rod* rod = player.getRod();
    std::cout << player.getNome()
              << " | Carteira: $" << player.getCarteira()
              << " | Estoque: " << player.getEstoque().size() << "/" << Pescador::CAPACIDADE_ESTOQUE;
    if (rod != nullptr)
        std::cout << " | Vara: " << rod->getNome()
                  << " (prof. " << rod->getDepthLimit() << ")";
    std::cout << '\n';

    if (screen->getCurrentID() == "lago")
        std::cout << "[WASD] move o anzol  [E] volta para a vila";
    else
        std::cout << "[WASD] anda  [I] inventario  [Q] sair   O = lago  $ = mercado  L = loja";
    std::cout << '\n';

    if (!mensagem.empty())
        std::cout << "\n>> " << mensagem << '\n';
    std::cout.flush();
}

void Jogo::run_fishing_game() {
    // ir ao lago faz o tempo passar: o estoque envelhece
    player.envelhecerEstoque();
    unsigned int podres = player.contarPodres();

    screen->changeMapa("lago");
    screen->spawnPeixes();
    player.setPos(screen->getSpawn());
    player.setAncora(screen->getSpawn());

    mensagem = "Encoste o anzol (J) em um peixe para fisga-lo.";
    if (podres > 0)
        mensagem += " Voce tem " + std::to_string(podres) + " peixe(s) podre(s) no estoque!";

    // LAMBDA que captura 'this': tenta fisgar o peixe que está no anzol
    auto tentarFisgar = [this]() {
        Peixe pescado(Vector2(), Dificuldade::Easy);
        if (!screen->pescarPeixeEm(player.getPos(), pescado)) return;

        try {
            player.guardarPeixe(pescado);
            mensagem = "Voce pescou um " + pescado.getNome() + "! (vale $" +
                       std::to_string((int)pescado.getValor()) + ")";
            if (pescado.getDificuldade() == Dificuldade::Hard)
                venceu = true;
        } catch (const EstoqueCheioException& e) {
            mensagem = std::string(e.what()) + " O " + pescado.getNome() + " escapou.";
        }
    };

    while (!venceu) {
        screen->printMapa();
        printHUD();

        char comando = lerTecla();
        if (comando == 'e' || comando == 'E') break;

        player.atualizarPos(comando, screen->getMapa(), '~');
        tentarFisgar();
        screen->fishSwim();
        tentarFisgar();

        if (screen->quantidadePeixes() == 0 && !venceu) {
            screen->printMapa();
            printHUD();
            std::cout << "\nO lago ficou vazio por hoje. Voltando para a vila..." << std::endl;
            esperarTecla();
            break;
        }
    }

    if (!venceu)
        mensagem = "De volta a vila.";
    screen->changeMapa("vila");
}

void Jogo::mercado() {
    system("clear");
    std::cout << "=== MERCADO ===\n" << std::endl;

    if (player.getEstoque().empty()) {
        std::cout << "Voce nao tem nenhum peixe para vender. Va pescar!" << std::endl;
        esperarTecla();
        return;
    }

    for (const auto& peixe : player.getEstoque())
        peixe.printFish();

    unsigned int podres = player.contarPodres();
    if (podres > 0)
        std::cout << "\n" << podres << " peixe(s) podre(s) vao para o lixo." << std::endl;

    std::cout << "\nVender tudo? (s/n) ";
    char c = lerTecla();
    std::cout << c << std::endl;
    if (c == 's' || c == 'S') {
        float ganho = player.venderEstoque();
        mensagem = "Voce vendeu o estoque por $" + std::to_string((int)ganho) + ".";
    } else {
        mensagem = "Volte quando quiser vender.";
    }
}

void Jogo::loja() {
    // Catálogo da loja: cada oferta guarda uma LAMBDA que fabrica a vara
    struct Oferta {
        std::string nome;
        float preco;
        unsigned int profundidade;
        std::function<std::unique_ptr<Rod>()> criar;
    };

    const std::vector<Oferta> ofertas = {
        {"Vara de Carbono", 40, 6, []() {
             return std::make_unique<Rod>("Vara de Carbono",
                 "Leve e resistente. Alcanca as tilapias do meio do lago.", 40, 6, 10);
         }},
        {"Vara Lendaria", 150, 9, []() {
             return std::make_unique<Rod>("Vara Lendaria",
                 "Dizem que com ela da para pescar ate o Dourado.", 150, 9, 14);
         }},
    };

    system("clear");
    std::cout << "=== LOJA DE VARAS ===   Carteira: $" << player.getCarteira() << "\n" << std::endl;
    for (unsigned int i = 0; i < ofertas.size(); i++)
        std::cout << "[" << i + 1 << "] " << ofertas[i].nome << " - $" << ofertas[i].preco
                  << " (profundidade " << ofertas[i].profundidade << ")" << std::endl;
    std::cout << "[outra tecla] sair" << std::endl;

    char c = lerTecla();
    unsigned int escolha = c - '1';
    if (escolha >= ofertas.size()) {
        mensagem = "Volte sempre!";
        return;
    }

    const Oferta& oferta = ofertas[escolha];
    if (player.getRod() != nullptr && player.getRod()->getDepthLimit() >= oferta.profundidade) {
        mensagem = "Voce ja tem uma vara igual ou melhor que essa.";
        return;
    }

    try {
        player.comprarRod(oferta.criar());
        mensagem = "Voce comprou a " + oferta.nome + "! Ela ja esta equipada.";
    } catch (const SaldoInsuficienteException& e) {
        mensagem = e.what();
    }
}

void Jogo::run_game() {
    screen->changeMapa("vila");
    player.setPos(screen->getSpawn());
    mensagem = "Va ate o lago (O) para pescar.";

    while (player.isAlive() && !venceu) {
        screen->printMapa();
        printHUD();

        char comando = lerTecla();
        if (comando == 'q' || comando == 'Q') break;
        if (comando == 'i' || comando == 'I') {
            system("clear");
            player.printInventario();
            esperarTecla();
            continue;
        }

        Vector2 anterior = player.getPos();
        player.atualizarPos(comando, screen->getMapa());

        switch (screen->objetoEm(player.getPos())) {
            case 'O': run_fishing_game(); break;
            case '$': mercado(); break;
            case 'L': loja(); break;
            default: continue;
        }
        // depois de interagir, o jogador volta um passo para não entrar de novo
        player.setPos(anterior);
    }

    system("clear");
    if (venceu) {
        std::cout << "PARABENS, " << player.getNome() << "! Voce pescou o Dourado!\n" << std::endl;
    }
    std::cout << "Fim de jogo. Obrigado por jogar Fishz!" << std::endl;
    player.printInventario();
}
