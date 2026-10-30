#include "pescador.h"
#include "excecoes.h"

#include <algorithm>
#include <iostream>
#include <numeric>

Pescador::Pescador()
    : nome("Pescador"), vida_atual(ENERGIA_MAX), posicao(), ancora(),
      equiped_rod(nullptr), carteira(0) {}

Pescador::Pescador(const std::string& name) : Pescador() {
    nome = name;
}

std::string Pescador::getNome() const { return nome; }
void Pescador::setNome(const std::string& name) { nome = name; }

unsigned int Pescador::getEnergia() const { return vida_atual; }

void Pescador::gastarEnergia(unsigned int qtd) {
    if (qtd > vida_atual)
        throw SemEnergiaException(qtd, vida_atual);
    vida_atual -= qtd;
}

void Pescador::descansar() { vida_atual = ENERGIA_MAX; }

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

void Pescador::addIsca(std::unique_ptr<Isca> isca) {
    // LAMBDA + dynamic_cast: procura no inventário uma isca com o mesmo nome
    auto it = std::find_if(inventario.begin(), inventario.end(),
                           [&isca](const std::unique_ptr<Item>& item) {
                               auto* existente = dynamic_cast<Isca*>(item.get());
                               return existente != nullptr && existente->getNome() == isca->getNome();
                           });
    if (it != inventario.end())
        static_cast<Isca*>(it->get())->adicionar(isca->getQuantidade());
    else
        inventario.push_back(std::move(isca));
}

const Rod* Pescador::getRod() const {
    return equiped_rod;
}

// POLIMORFISMO: a loja vende Item, e o pescador descobre o tipo real
// com dynamic_cast para saber se equipa (vara) ou empilha (isca)
void Pescador::comprar(std::unique_ptr<Item> item) {
    if (item->getValor() > carteira)
        throw SaldoInsuficienteException(item->getValor(), carteira);
    carteira -= item->getValor();

    if (auto* rod = dynamic_cast<Rod*>(item.get())) {
        item.release();
        addRod(std::unique_ptr<Rod>(rod));
    } else if (auto* isca = dynamic_cast<Isca*>(item.get())) {
        item.release();
        addIsca(std::unique_ptr<Isca>(isca));
    } else {
        inventario.push_back(std::move(item));
    }
}

unsigned int Pescador::usarIsca() {
    // escolhe a isca com maior bônus que ainda tem unidades
    Isca* melhor = nullptr;
    for (auto& item : inventario) {
        auto* isca = dynamic_cast<Isca*>(item.get());
        if (isca != nullptr && isca->getQuantidade() > 0 &&
            (melhor == nullptr || isca->getBonus() > melhor->getBonus()))
            melhor = isca;
    }
    if (melhor == nullptr) return 0;

    melhor->usar();
    unsigned int bonus = melhor->getBonus();

    // LAMBDA com remove_if: tira do inventário as iscas que acabaram
    inventario.erase(std::remove_if(inventario.begin(), inventario.end(),
                                    [](const std::unique_ptr<Item>& item) {
                                        auto* isca = dynamic_cast<Isca*>(item.get());
                                        return isca != nullptr && isca->getQuantidade() == 0;
                                    }),
                     inventario.end());
    return bonus;
}

unsigned int Pescador::getTotalIscas() const {
    unsigned int total = 0;
    for (const auto& item : inventario)
        if (auto* isca = dynamic_cast<const Isca*>(item.get()))
            total += isca->getQuantidade();
    return total;
}

void Pescador::printInventario() const {
    std::cout << "\n=== Inventario de " << nome << " ===" << std::endl;
    std::cout << "Carteira: $" << carteira << " | Energia: " << vida_atual
              << "/" << ENERGIA_MAX << std::endl;

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

unsigned int Pescador::contarPeixes(Dificuldade tipo) const {
    return std::count_if(estoque.begin(), estoque.end(), [tipo](const Peixe& p) {
        return p.getDificuldade() == tipo && p.getValidade() != Validade::Podre;
    });
}

bool Pescador::entregarPeixes(Dificuldade tipo, unsigned int qtd) {
    if (contarPeixes(tipo) < qtd) return false;

    unsigned int removidos = 0;
    for (auto it = estoque.begin(); it != estoque.end() && removidos < qtd;) {
        if (it->getDificuldade() == tipo && it->getValidade() != Validade::Podre) {
            it = estoque.erase(it);
            removidos++;
        } else {
            ++it;
        }
    }
    return true;
}

const std::vector<Peixe>& Pescador::getEstoque() const {
    return estoque;
}

float Pescador::getCarteira() const {
    return carteira;
}

void Pescador::receber(float valor) {
    carteira += valor;
}
