#include "pescador.h"
#include "excecoes.h"

#include <algorithm>
#include <iostream>
#include <numeric>

Pescador::Pescador()
    : nome("Pescador"), vida_atual(100), posicao(), ancora(),
      equiped_rod(nullptr), carteira(0) {}

Pescador::Pescador(const std::string& name) : Pescador() {
    nome = name;
}

std::string Pescador::getNome() const { return nome; }
void Pescador::setNome(const std::string& name) { nome = name; }
bool Pescador::isAlive() const { return vida_atual > 0; }

Vector2 Pescador::getPos() const { return posicao; }
unsigned int Pescador::getPosX() const { return posicao.x; }
unsigned int Pescador::getPosY() const { return posicao.y; }
void Pescador::setPos(Vector2 pos) { posicao = pos; }
void Pescador::setAncora(Vector2 pos) { ancora = pos; }

// Calcula para onde o comando (WASD) levaria a partir de 'pos'.
// Devolve false se sairia do mapa (evita underflow do unsigned).
static bool calcularDestino(char comando, Vector2 pos,
                            const std::vector<std::vector<char>>& mapa, Vector2& destino) {
    destino = pos;
    switch (comando) {
        case 'w': case 'W':
            if (pos.y == 0) return false;
            destino.y--;
            break;
        case 's': case 'S':
            destino.y++;
            break;
        case 'a': case 'A':
            if (pos.x == 0) return false;
            destino.x--;
            break;
        case 'd': case 'D':
            destino.x++;
            break;
        default:
            return false;
    }
    if (destino.y >= mapa.size()) return false;
    if (destino.x >= mapa[destino.y].size()) return false;
    return true;
}

void Pescador::atualizarPos(char comando, const std::vector<std::vector<char>>& mapa) {
    Vector2 destino;
    if (!calcularDestino(comando, posicao, mapa, destino)) return;

    if (mapa[destino.y][destino.x] == ' ')
        posicao = destino;
}

void Pescador::atualizarPos(char comando, const std::vector<std::vector<char>>& mapa, char rpos) {
    Vector2 destino;
    if (!calcularDestino(comando, posicao, mapa, destino)) return;
    if (mapa[destino.y][destino.x] != rpos) return;

    // limites da vara: profundidade e distância lateral a partir da âncora
    if (equiped_rod != nullptr) {
        unsigned int profundidade = destino.y - ancora.y;
        unsigned int distancia = destino.x > ancora.x ? destino.x - ancora.x
                                                      : ancora.x - destino.x;
        if (profundidade > equiped_rod->getDepthLimit()) return;
        if (distancia > equiped_rod->getDistanceLimit()) return;
    }
    posicao = destino;
}

void Pescador::addRod(std::unique_ptr<Rod> rod) {
    equiped_rod = rod.get();            // equipa a vara nova
    inventario.push_back(std::move(rod));  // e o inventário passa a ser o dono
}

const Rod* Pescador::getRod() const {
    return equiped_rod;
}

void Pescador::comprarRod(std::unique_ptr<Rod> rod) {
    if (rod->getValor() > carteira)
        throw SaldoInsuficienteException(rod->getValor(), carteira);

    carteira -= rod->getValor();
    addRod(std::move(rod));
}

void Pescador::printInventario() const {
    std::cout << "\n=== Inventario de " << nome << " ===" << std::endl;
    std::cout << "Carteira: $" << carteira << std::endl;

    std::cout << "\nItens:" << std::endl;
    for (const auto& item : inventario) {
        item->itemDescription();
        if (item.get() == equiped_rod) std::cout << "    (equipada)" << std::endl;
    }

    std::cout << "\nEstoque (" << estoque.size() << "/" << CAPACIDADE_ESTOQUE << "):" << std::endl;
    if (estoque.empty()) {
        std::cout << "  vazio" << std::endl;
        return;
    }

    // LAMBDA: mostra o estoque ordenado do peixe mais valioso para o menos valioso
    std::vector<Peixe> ordenado = estoque;
    std::sort(ordenado.begin(), ordenado.end(),
              [](const Peixe& a, const Peixe& b) {
                  return a.getPrecoVenda() > b.getPrecoVenda();
              });
    for (const auto& peixe : ordenado)
        peixe.printFish();
}

void Pescador::guardarPeixe(const Peixe& peixe) {
    if (estoque.size() >= CAPACIDADE_ESTOQUE)
        throw EstoqueCheioException(CAPACIDADE_ESTOQUE);
    estoque.push_back(peixe);
}

void Pescador::envelhecerEstoque() {
    // LAMBDA com std::for_each
    std::for_each(estoque.begin(), estoque.end(), [](Peixe& p) { p.envelhecer(); });
}

unsigned int Pescador::contarPodres() const {
    // LAMBDA com std::count_if
    return std::count_if(estoque.begin(), estoque.end(), [](const Peixe& p) {
        return p.getValidade() == Validade::Podre;
    });
}

float Pescador::venderEstoque() {
    // LAMBDA com std::accumulate: soma o preço de venda de todo o estoque
    float ganho = std::accumulate(estoque.begin(), estoque.end(), 0.0f,
                                  [](float soma, const Peixe& p) {
                                      return soma + p.getPrecoVenda();
                                  });
    carteira += ganho;
    estoque.clear();  // peixes podres são jogados fora junto
    return ganho;
}

const std::vector<Peixe>& Pescador::getEstoque() const {
    return estoque;
}

float Pescador::getCarteira() const {
    return carteira;
}
