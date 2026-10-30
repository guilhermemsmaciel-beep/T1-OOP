// Testes automáticos da lógica do jogo (não precisam de terminal interativo).
// Rode com: make test

#include <cassert>
#include <iostream>
#include <memory>

#include "excecoes.h"
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
        p.comprarRod(std::make_unique<Rod>("Carbono", "", 40, 6, 10));
    } catch (const SaldoInsuficienteException&) {
        lancou = true;
    }
    assert(lancou);
    assert(p.getRod()->getDepthLimit() == 3);  // continua com a vara antiga

    for (int i = 0; i < 8; i++) p.guardarPeixe(Peixe(Vector2(), Dificuldade::Easy));
    p.venderEstoque();  // $40
    p.comprarRod(std::make_unique<Rod>("Carbono", "", 40, 6, 10));
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

int main() {
    testaVector2();
    testaEnvelhecimentoEVenda();
    testaEstoqueCheio();
    testaCompraDeVara();
    testaMapa();
    testaLimiteDaVara();
    std::cout << "\nTodos os testes passaram!" << std::endl;
    return 0;
}
