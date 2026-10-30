#include "jogo.h"
#include "excecoes.h"
#include "fisgada.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

// O jogo roda com o terminal em modo "cbreak" (sem Enter e sem eco),
// assim cada tecla chega na hora. O modo normal só volta para digitar o nome.
void Jogo::modoJogo() {
    system("stty -icanon -echo");
}

void Jogo::modoTexto() {
    system("stty sane");
}

// LAMBDA: lê uma única tecla (espera até o jogador apertar)
static auto lerTecla = []() -> char {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;
    return c;
};

// Espera uma tecla por no máximo 'ms' milissegundos. Devolve -1 se nada foi apertado.
// Usada no minigame de fisgada, em que a barra precisa andar sozinha.
static int lerTeclaComTimeout(unsigned int ms) {
    fd_set leitura;
    FD_ZERO(&leitura);
    FD_SET(STDIN_FILENO, &leitura);
    timeval espera{0, static_cast<suseconds_t>(ms * 1000)};

    if (select(STDIN_FILENO + 1, &leitura, nullptr, nullptr, &espera) > 0)
        return lerTecla();
    return -1;
}

static void esperarTecla() {
    std::cout << "\n(aperte qualquer tecla para continuar)" << std::endl;
    lerTecla();
}

Jogo::Jogo(Pescador& p)
    : screen(nullptr), npc(nullptr), player(p), dia(1), venceu(false),
      rng(std::random_device{}()) {}

void Jogo::game_preparation() {
    screen = std::make_unique<Mapa>(player);
    mapas.push_back("assets/mapas/vila.txt");
    mapas.push_back("assets/mapas/lago.txt");
    screen->loadMapas(mapas);

    // o NPC fica onde está o 'N' no mapa da vila
    screen->changeMapa("vila");
    npc = std::make_unique<Npc>(screen->localizarObjeto('N'), "Seu Ze");
}

void Jogo::game_intro() {
    limparTela();
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

    std::cout << "\nE leve tambem umas minhocas. Cada fisgada gasta uma isca.\n" << std::endl;
    player.addIsca(std::make_unique<Isca>("Minhoca", "Isca simples. Aumenta um pouco a zona de fisgada.",
                                          5, 2, 5));

    std::cout << "Seu objetivo: pescar um DOURADO, o peixe que vive no fundo do lago." << std::endl;
    std::cout << "- Ir ao lago gasta energia. Durma em casa (H) para recuperar." << std::endl;
    std::cout << "- Ao dormir, um dia passa e os peixes guardados envelhecem." << std::endl;
    std::cout << "- Venda no mercado ($), compre na loja (L) e fale com o Seu Ze (N)." << std::endl;
    esperarTecla();
}

void Jogo::printHUD() const {
    const Rod* rod = player.getRod();
    std::cout << player.getNome() << " | Dia " << dia
              << " | Energia " << player.getEnergia() << "/" << Pescador::ENERGIA_MAX
              << " | $" << player.getCarteira()
              << " | Estoque " << player.getEstoque().size() << "/" << Pescador::CAPACIDADE_ESTOQUE
              << " | Iscas " << player.getTotalIscas() << '\n';
    if (rod != nullptr)
        std::cout << "Vara: " << rod->getNome() << " (prof. " << rod->getDepthLimit()
                  << ", alcance " << rod->getDistanceLimit() << ")\n";

    if (screen->getCurrentID() == "lago")
        std::cout << "[WASD] move o anzol  [E] volta para a vila";
    else
        std::cout << "[WASD] anda  [I] inventario  [Q] sair\n"
                     "O lago  $ mercado  L loja  H casa  N Seu Ze";
    std::cout << '\n';

    if (!mensagem.empty())
        std::cout << "\n>> " << mensagem << '\n';
    std::cout.flush();
}

// Minigame de fisgada. Devolve true se o jogador puxou o peixe.
bool Jogo::puxarPeixe(const Peixe& peixe) {
    unsigned int bonus = player.usarIsca();
    Fisgada fisgada(peixe.getDificuldade(), bonus, rng);

    tcflush(STDIN_FILENO, TCIFLUSH);  // descarta teclas apertadas antes do minigame

    limparTela();
    std::cout << "Algo mordeu a isca! E um " << peixe.getNome()
              << " [" << dificuldadeToString(peixe.getDificuldade()) << "]\n\n";
    if (bonus > 0)
        std::cout << "Sua isca deu +" << bonus << " na zona verde.\n";
    else
        std::cout << "Voce esta sem isca: a zona verde e menor.\n";
    std::cout << "Aperte ESPACO quando o marcador | estiver na zona verde!\n\n";

    while (!fisgada.esgotou()) {
        std::cout << "\r  " << fisgada.desenhar() << std::flush;

        int tecla = lerTeclaComTimeout(fisgada.getIntervaloMs());
        if (tecla == ' ') {
            bool acertou = fisgada.dentroDaZona();
            std::cout << "\n\n";
            return acertou;
        }
        fisgada.avancar();
    }
    std::cout << "\n\n";
    return false;
}

void Jogo::run_fishing_game() {
    screen->changeMapa("lago");
    screen->spawnPeixes();
    player.setPos(screen->getSpawn());
    player.setAncora(screen->getSpawn());

    mensagem = "Encoste o anzol (J) em um peixe para fisga-lo.";

    // LAMBDA que captura 'this': se o anzol encostou num peixe, abre a fisgada
    auto tentarFisgar = [this]() {
        Peixe pescado(Vector2(), Dificuldade::Easy);
        if (!screen->pescarPeixeEm(player.getPos(), pescado)) return;

        if (!puxarPeixe(pescado)) {
            mensagem = "O " + pescado.getNome() + " escapou!";
            return;
        }
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
    limparTela();
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
    // Catálogo: cada oferta guarda uma LAMBDA que fabrica o item.
    // Varas e iscas são vendidas como Item (polimorfismo).
    struct Oferta {
        std::string nome;
        float preco;
        unsigned int profundidade;  // 0 = não é vara
        std::function<std::unique_ptr<Item>()> criar;
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
        {"Minhoca x5", 5, 0, []() {
             return std::make_unique<Isca>("Minhoca",
                 "Isca simples. Aumenta um pouco a zona de fisgada.", 5, 2, 5);
         }},
        {"Isca Artificial x3", 20, 0, []() {
             return std::make_unique<Isca>("Isca Artificial",
                 "Brilha na agua. Aumenta bastante a zona de fisgada.", 20, 5, 3);
         }},
    };

    limparTela();
    std::cout << "=== LOJA ===   Carteira: $" << player.getCarteira() << "\n" << std::endl;
    for (unsigned int i = 0; i < ofertas.size(); i++) {
        std::cout << "[" << i + 1 << "] " << ofertas[i].nome << " - $" << ofertas[i].preco;
        if (ofertas[i].profundidade > 0)
            std::cout << " (profundidade " << ofertas[i].profundidade << ")";
        std::cout << std::endl;
    }
    std::cout << "[outra tecla] sair" << std::endl;

    char c = lerTecla();
    unsigned int escolha = c - '1';
    if (escolha >= ofertas.size()) {
        mensagem = "Volte sempre!";
        return;
    }

    const Oferta& oferta = ofertas[escolha];
    if (oferta.profundidade > 0 && player.getRod() != nullptr &&
        player.getRod()->getDepthLimit() >= oferta.profundidade) {
        mensagem = "Voce ja tem uma vara igual ou melhor que essa.";
        return;
    }

    try {
        player.comprar(oferta.criar());
        mensagem = "Voce comprou: " + oferta.nome + "!";
    } catch (const SaldoInsuficienteException& e) {
        mensagem = e.what();
    }
}

void Jogo::dormir() {
    dia++;
    player.descansar();
    player.envelhecerEstoque();  // a noite passa e os peixes guardados envelhecem

    mensagem = "Voce dormiu. Bom dia! Comecou o dia " + std::to_string(dia) + ".";
    unsigned int podres = player.contarPodres();
    if (podres > 0)
        mensagem += " " + std::to_string(podres) + " peixe(s) do estoque apodreceram.";
}

void Jogo::falarComNpc() {
    limparTela();
    std::cout << "=== " << npc->getNome() << " ===\n\n";
    std::cout << "\"" << npc->fala() << "\"\n" << std::endl;

    if (!npc->temMissao()) {
        esperarTecla();
        mensagem = "Voce ja ajudou o " + npc->getNome() + " com tudo.";
        return;
    }

    const Missao& missao = npc->missaoAtual();
    unsigned int tenho = player.contarPeixes(missao.tipo);
    std::cout << "Missao: " << missao.pedido << "\n";
    std::cout << "Recompensa: $" << missao.recompensa << "\n";
    std::cout << "Voce tem " << tenho << "/" << missao.quantidade
              << " (peixes podres nao contam)\n" << std::endl;

    if (tenho < missao.quantidade) {
        mensagem = "Missao do " + npc->getNome() + ": " + missao.pedido;
        esperarTecla();
        return;
    }

    std::cout << "Entregar os peixes? (s/n) ";
    char c = lerTecla();
    std::cout << c << std::endl;
    if (c != 's' && c != 'S') {
        mensagem = "O " + npc->getNome() + " vai esperar.";
        return;
    }

    float recompensa = missao.recompensa;
    player.entregarPeixes(missao.tipo, missao.quantidade);
    player.receber(recompensa);
    npc->concluirMissao();
    mensagem = "Missao concluida! O " + npc->getNome() + " te pagou $" +
               std::to_string((int)recompensa) + ".";
}

void Jogo::run_game() {
    screen->changeMapa("vila");
    player.setPos(screen->getSpawn());
    mensagem = "Va ate o lago (O) para pescar, ou fale com o Seu Ze (N).";
    bool sair = false;

    while (!venceu && !sair) {
        screen->printMapa();
        printHUD();

        char comando = lerTecla();
        if (comando == 'q' || comando == 'Q') break;
        if (comando == 'i' || comando == 'I') {
            limparTela();
            player.printInventario();
            esperarTecla();
            continue;
        }

        Vector2 anterior = player.getPos();
        player.atualizarPos(comando, screen->getMapa());

        switch (screen->objetoEm(player.getPos())) {
            case 'O':
                try {
                    player.gastarEnergia(CUSTO_PESCA);
                    run_fishing_game();
                } catch (const SemEnergiaException& e) {
                    mensagem = std::string(e.what()) + " Durma em casa (H).";
                }
                break;
            case '$': mercado(); break;
            case 'L': loja(); break;
            case 'H': dormir(); break;
            case 'N': falarComNpc(); break;
            default: continue;
        }
        // depois de interagir, o jogador volta um passo para não entrar de novo
        player.setPos(anterior);
    }

    limparTela();
    if (venceu) {
        std::cout << "PARABENS, " << player.getNome() << "! Voce pescou o Dourado em "
                  << dia << " dia(s)!\n" << std::endl;
    }
    std::cout << "Fim de jogo. Obrigado por jogar Fishz!" << std::endl;
    std::cout << "Missoes concluidas para o " << npc->getNome() << ": "
              << npc->missoesConcluidas() << std::endl;
    player.printInventario();
}
