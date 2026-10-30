#ifndef PESCADOR_H
#define PESCADOR_H

#include <string>
#include <vector>
#include <memory>

#include "gtools.h"
#include "peixe.h"
#include "item.h"
#include "rod.h"

class Pescador {
private:
    std::string nome;
    unsigned int vida_atual;
    Vector2 posicao;
    Vector2 ancora;                              // de onde a linha sai no lago
    std::vector<Peixe> estoque;                  // peixes pescados
    Rod* equiped_rod;                            // vara em uso (aponta para algo do inventario)
    std::vector<std::unique_ptr<Item>> inventario;  // o pescador é dono dos itens
    float carteira;

public:
    static const unsigned int CAPACIDADE_ESTOQUE = 10;

    Pescador();
    explicit Pescador(const std::string& name);

    std::string getNome() const;
    void setNome(const std::string& name);
    bool isAlive() const;

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

    // Itens e vara
    void addRod(std::unique_ptr<Rod> rod);
    const Rod* getRod() const;
    void comprarRod(std::unique_ptr<Rod> rod);   // lança SaldoInsuficienteException
    void printInventario() const;

    // Peixes
    void guardarPeixe(const Peixe& peixe);       // lança EstoqueCheioException
    void envelhecerEstoque();
    unsigned int contarPodres() const;
    float venderEstoque();                       // devolve quanto ganhou
    const std::vector<Peixe>& getEstoque() const;

    float getCarteira() const;
};

#endif
