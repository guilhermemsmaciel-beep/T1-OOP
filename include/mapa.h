#ifndef MAPA_H
#define MAPA_H

#include <map>
#include <random>
#include <string>
#include <vector>

#include "gtools.h"
#include "object.h"
#include "peixe.h"
#include "pescador.h"

// Símbolos usados nos arquivos de mapa (assets/mapas/*.txt)
//   ' ' chão livre        '~' água          'P' onde o jogador nasce
//   'O' lago (pescar)     '$' mercado       'L' loja
//   'H' casa (dormir)     'N' NPC
//   qualquer outro caractere é parede/decoração
class Mapa {
private:
    std::map<std::string, std::vector<std::vector<char>>> game_maps;  // todos os mapas carregados
    std::vector<std::vector<char>> current_mapa;                      // mapa na tela
    std::string current_ID;
    Pescador& player;
    std::vector<Object> objetos;
    std::vector<Peixe> peixes;
    Vector2 spawn;
    std::mt19937 rng;

    void limitesDaAgua(unsigned int linha, unsigned int& x_min, unsigned int& x_max) const;

public:
    explicit Mapa(Pescador& p);

    const std::vector<std::vector<char>>& getMapa() const;
    std::string getCurrentID() const;
    Vector2 getSpawn() const;

    Vector2 getObjectPos(unsigned int index) const;  // lança MapaInvalidoException
    Vector2 localizarObjeto(char simbolo) const;      // lança MapaInvalidoException
    char objetoEm(Vector2 pos) const;                // '\0' se não houver objeto

    void loadMapas(const std::vector<std::string>& arquivos);  // lança ArquivoMapaException
    void loadMapa();                                 // carrega current_ID na tela
    void changeMapa(const std::string& id);          // lança MapaInvalidoException

    // Pescaria
    void spawnPeixes();
    void fishSwim();
    bool pescarPeixeEm(Vector2 pos, Peixe& pescado);
    unsigned int quantidadePeixes() const;

    void printMapa() const;
};

#endif
