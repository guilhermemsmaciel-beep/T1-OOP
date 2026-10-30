#ifndef PESCADOR_H
#define PESCADOR_H

#include <string>
#include <vector>
#include <memory>

#include "gtools.h"
#include "peixe.h"
#include "item.h"
#include "rod.h"
#include "isca.h"

class Pescador {
private:
    std::string nome;
    unsigned int vida_atual;                     // energia do pescador (0 a ENERGIA_MAX)
    Vector2 posicao;
    Vector2 ancora;                              // de onde a linha sai no lago
    std::vector<Peixe> estoque;                  // peixes pescados
    Rod* equiped_rod;                            // vara em uso (aponta para algo do inventario)
    std::vector<std::unique_ptr<Item>> inventario;  // o pescador é dono dos itens
    float carteira;

public:
    static const unsigned int CAPACIDADE_ESTOQUE = 10;
    static const unsigned int ENERGIA_MAX = 100;

    Pescador();
    explicit Pescador(const std::string& name);

    std::string getNome() const;
    void setNome(const std::string& name);

    // Energia
    unsigned int getEnergia() const;
    void gastarEnergia(unsigned int qtd);        // lança SemEnergiaException
    void descansar();

    // Posição
    Vector2 getPos() const;
    unsigned int getPosX() const;
    unsigned int getPosY() const;
    void setPos(Vector2 pos);
    void setAncora(Vector2 pos);

    // Andando em terra: só entra em células vazias (' ')
    void atualizarPos(char comando, const std::vector<std::vector<char>>& mapa);
    // Pescando: o anzol só anda em células 'rpos' (água) e dentro do alcance da vara
    void atualizarPos(char comando, const std::vector<std::vector<char>>& mapa, char rpos);

    // Itens, vara e iscas
    void addRod(std::unique_ptr<Rod> rod);
    void addIsca(std::unique_ptr<Isca> isca);    // empilha com iscas iguais
    const Rod* getRod() const;
    void comprar(std::unique_ptr<Item> item);    // lança SaldoInsuficienteException
    unsigned int usarIsca();                     // gasta a melhor isca e devolve o bônus
    unsigned int getTotalIscas() const;
    void printInventario() const;

    // Peixes
    void guardarPeixe(const Peixe& peixe);       // lança EstoqueCheioException
    void envelhecerEstoque();
    unsigned int contarPodres() const;
    float venderEstoque();                       // devolve quanto ganhou
    unsigned int contarPeixes(Dificuldade tipo) const;  // só os que não estão podres
    bool entregarPeixes(Dificuldade tipo, unsigned int qtd);
    const std::vector<Peixe>& getEstoque() const;

    float getCarteira() const;
    void receber(float valor);
};

#endif
