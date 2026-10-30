// Testes automáticos da lógica do jogo (não precisam de terminal interativo).
// Rode com: make test

#include <cassert>
#include <iostream>
#include <memory>

#include "excecoes.h"
#include "fisgada.h"
#include "isca.h"
#include "npc.h"
#include "mapa.h"
#include "pescador.h"
#include "rod.h"

static void testaVector2() {
    assert(Vector2(1, 2) == Vector2(1, 2));
    assert(Vector2(1, 2) != Vector2(2, 1));
    std::cout << "[ok] Vector2" << std::endl;
}

static void testaEnvelhecimentoEVenda() {
    Pescador p("Teste");
    p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));    // Lambari $5
    p.guardarPeixe(Peixe(Vector2(), Dificuldade::Medium));  // Tilapia $15

    p.envelhecerEstoque();  // Passado: metade do preço
    assert(p.contarPodres() == 0);
    p.envelhecerEstoque();  // Podre: não vale nada
    assert(p.contarPodres() == 2);

    p.guardarPeixe(Peixe(Vector2(), Dificuldade::Hard));    // Dourado fresco $50
    float ganho = p.venderEstoque();
    assert(ganho == 50);
    assert(p.getCarteira() == 50);
    assert(p.getEstoque().empty());
    std::cout << "[ok] envelhecimento e venda" << std::endl;
}

static void testaEstoqueCheio() {
    Pescador p;
    for (unsigned int i = 0; i < Pescador::CAPACIDADE_ESTOQUE; i++)
        p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));

    bool lancou = false;
    try {
        p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));
    } catch (const EstoqueCheioException&) {
        lancou = true;
    }
    assert(lancou);
    std::cout << "[ok] EstoqueCheioException" << std::endl;
}

static void testaCompraDeVara() {
    Pescador p;
    p.addRod(std::make_unique<Rod>());

    bool lancou = false;
    try {
        p.comprar(std::make_unique<Rod>("Carbono", "", 40, 6, 10));
    } catch (const SaldoInsuficienteException&) {
        lancou = true;
    }
    assert(lancou);
    assert(p.getRod()->getDepthLimit() == 3);  // continua com a vara antiga

    for (int i = 0; i < 8; i++) p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));
    p.venderEstoque();  // $40
    p.comprar(std::make_unique<Rod>("Carbono", "", 40, 6, 10));
    assert(p.getRod()->getDepthLimit() == 6);
    assert(p.getCarteira() == 0);
    std::cout << "[ok] compra de vara e SaldoInsuficienteException" << std::endl;
}

static void testaMapa() {
    Pescador p;
    Mapa mapa(p);

    bool lancou = false;
    try {
        mapa.loadMapas({"assets/mapas/nao_existe.txt"});
    } catch (const ArquivoMapaException&) {
        lancou = true;
    }
    assert(lancou);

    mapa.loadMapas({"assets/mapas/vila.txt", "assets/mapas/lago.txt"});

    lancou = false;
    try {
        mapa.changeMapa("deserto");
    } catch (const MapaInvalidoException&) {
        lancou = true;
    }
    assert(lancou);

    mapa.changeMapa("vila");
    assert(mapa.objetoEm(mapa.getObjectPos(0)) != '\0');

    lancou = false;
    try {
        mapa.getObjectPos(99);
    } catch (const MapaInvalidoException&) {
        lancou = true;
    }
    assert(lancou);

    mapa.changeMapa("lago");
    mapa.spawnPeixes();
    assert(mapa.quantidadePeixes() > 0);
    std::cout << "[ok] Mapa, ArquivoMapaException e MapaInvalidoException" << std::endl;
}

static void testaLimiteDaVara() {
    Pescador p;
    p.addRod(std::make_unique<Rod>());  // profundidade 3
    Mapa mapa(p);
    mapa.loadMapas({"assets/mapas/lago.txt"});
    mapa.changeMapa("lago");

    p.setPos(mapa.getSpawn());
    p.setAncora(mapa.getSpawn());
    for (int i = 0; i < 10; i++)
        p.atualizarPos('s', mapa.getMapa(), '~');
    assert(p.getPosY() == mapa.getSpawn().y + 3);
    std::cout << "[ok] limite de profundidade da vara" << std::endl;
}

static void testaIscas() {
    Pescador p;
    assert(p.usarIsca() == 0);  // sem isca, sem bônus

    p.addIsca(std::make_unique<Isca>("Minhoca", "", 5, 2, 2));
    p.addIsca(std::make_unique<Isca>("Minhoca", "", 5, 2, 1));  // empilha
    p.addIsca(std::make_unique<Isca>("Isca Artificial", "", 20, 5, 1));
    assert(p.getTotalIscas() == 4);

    assert(p.usarIsca() == 5);  // usa primeiro a melhor
    assert(p.usarIsca() == 2);  // artificial acabou, passa para minhoca
    assert(p.getTotalIscas() == 2);
    std::cout << "[ok] iscas (empilhar, usar a melhor, remover quando acaba)" << std::endl;
}

static void testaCompraPolimorfica() {
    Pescador p;
    p.receber(100);
    std::unique_ptr<Item> isca = std::make_unique<Isca>("Minhoca", "", 5, 2, 5);
    p.comprar(std::move(isca));                     // vira isca no inventário
    assert(p.getTotalIscas() == 5);

    std::unique_ptr<Item> vara = std::make_unique<Rod>("Carbono", "", 40, 6, 10);
    p.comprar(std::move(vara));                     // vira a vara equipada
    assert(p.getRod()->getDepthLimit() == 6);
    assert(p.getCarteira() == 55);
    std::cout << "[ok] compra polimorfica (Item -> Rod / Isca)" << std::endl;
}

static void testaEnergia() {
    Pescador p;
    for (int i = 0; i < 5; i++) p.gastarEnergia(20);
    assert(p.getEnergia() == 0);

    bool lancou = false;
    try {
        p.gastarEnergia(20);
    } catch (const SemEnergiaException&) {
        lancou = true;
    }
    assert(lancou);
    p.descansar();
    assert(p.getEnergia() == Pescador::ENERGIA_MAX);
    std::cout << "[ok] energia e SemEnergiaException" << std::endl;
}

static void testaMissoes() {
    Npc npc(Vector2(1, 1), "Seu Ze");
    Pescador p;
    assert(npc.temMissao());
    const Missao& m = npc.missaoAtual();
    assert(m.tipo == Dificuldade::Easy && m.quantidade == 3);

    p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));
    p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));
    assert(!p.entregarPeixes(Dificuldade::Easy, 3));  // só tem 2
    p.guardarPeixe(Peixe(Vector2(), Dificuldade::Medium));
    p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));
    assert(p.entregarPeixes(Dificuldade::Easy, 3));
    assert(p.getEstoque().size() == 1);              // sobrou a tilápia
    npc.concluirMissao();
    assert(npc.missoesConcluidas() == 1);

    npc.concluirMissao();
    assert(!npc.temMissao());
    std::cout << "[ok] NPC e missoes" << std::endl;
}

static void testaFisgada() {
    std::mt19937 rng(42);
    Fisgada facil(Dificuldade::Easy, 0, rng);
    Fisgada dificil(Dificuldade::Hard, 0, rng);
    Fisgada comIsca(Dificuldade::Hard, 5, rng);
    assert(facil.getZonaTamanho() > dificil.getZonaTamanho());
    assert(comIsca.getZonaTamanho() == dificil.getZonaTamanho() + 5);
    assert(facil.getIntervaloMs() > dificil.getIntervaloMs());

    // o marcador passa pela zona verde em algum momento
    bool passouNaZona = false;
    while (!facil.esgotou()) {
        if (facil.dentroDaZona()) passouNaZona = true;
        facil.avancar();
    }
    assert(passouNaZona);
    std::cout << "[ok] fisgada (zona, velocidade e fuga do peixe)" << std::endl;
}

int main() {
    testaVector2();
    testaEnvelhecimentoEVenda();
    testaEstoqueCheio();
    testaCompraDeVara();
    testaMapa();
    testaLimiteDaVara();
    testaIscas();
    testaCompraPolimorfica();
    testaEnergia();
    testaMissoes();
    testaFisgada();
    std::cout << "\nTodos os testes passaram!" << std::endl;
    return 0;
}
