#include "mapa.h"
#include "excecoes.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>

Mapa::Mapa(Pescador& p) : player(p), rng(std::random_device{}()) {}

const std::vector<std::vector<char>>& Mapa::getMapa() const {
    return current_mapa;
}

std::string Mapa::getCurrentID() const {
    return current_ID;
}

Vector2 Mapa::getSpawn() const {
    return spawn;
}

Vector2 Mapa::getObjectPos(unsigned int index) const {
    try {
        return objetos.at(index).getPos();
    } catch (const std::out_of_range&) {
        // traduz o erro da STL para uma exceção do jogo
        throw MapaInvalidoException("objeto " + std::to_string(index) +
                                    " nao existe no mapa '" + current_ID + "'");
    }
}

char Mapa::objetoEm(Vector2 pos) const {
    // LAMBDA com std::find_if
    auto it = std::find_if(objetos.begin(), objetos.end(),
                           [pos](const Object& obj) { return obj.getPos() == pos; });
    return it != objetos.end() ? it->getSimbolo() : '\0';
}

// Lê cada arquivo e guarda no game_maps com o nome sem pasta e sem extensão.
// Ex.: "assets/mapas/vila.txt" -> "vila"
void Mapa::loadMapas(const std::vector<std::string>& arquivos) {
    for (const auto& caminho : arquivos) {
        std::ifstream arquivo(caminho);
        if (!arquivo.is_open())
            throw ArquivoMapaException(caminho);

        std::vector<std::vector<char>> grid;
        std::string linha;
        while (std::getline(arquivo, linha)) {
            if (!linha.empty() && linha.back() == '\r') linha.pop_back();  // arquivos do Windows
            grid.emplace_back(linha.begin(), linha.end());
        }
        if (grid.empty())
            throw ArquivoMapaException(caminho + " (arquivo vazio)");

        std::string id = caminho.substr(caminho.find_last_of('/') + 1);
        id = id.substr(0, id.find_last_of('.'));
        game_maps[id] = grid;
    }
}

void Mapa::changeMapa(const std::string& id) {
    if (game_maps.find(id) == game_maps.end())
        throw MapaInvalidoException("'" + id + "' nao foi carregado");
    current_ID = id;
    loadMapa();
}

void Mapa::loadMapa() {
    current_mapa = game_maps.at(current_ID);
    objetos.clear();
    peixes.clear();

    // Separa os símbolos especiais do grid: viram objetos ou o ponto de spawn
    for (unsigned int y = 0; y < current_mapa.size(); y++) {
        for (unsigned int x = 0; x < current_mapa[y].size(); x++) {
            char& c = current_mapa[y][x];
            if (c == 'O' || c == '$' || c == 'L') {
                objetos.emplace_back(Vector2(x, y), c);
                c = ' ';
            } else if (c == 'P') {
                spawn = Vector2(x, y);
                // no lago o spawn fica na água; em terra, no chão
                bool naAgua = (x > 0 && current_mapa[y][x - 1] == '~');
                c = naAgua ? '~' : ' ';
            }
        }
    }
}

void Mapa::limitesDaAgua(unsigned int linha, unsigned int& x_min, unsigned int& x_max) const {
    const auto& row = current_mapa.at(linha);
    x_min = 0;
    x_max = 0;
    bool achou = false;
    for (unsigned int x = 0; x < row.size(); x++) {
        if (row[x] == '~') {
            if (!achou) x_min = x;
            x_max = x;
            achou = true;
        }
    }
}

void Mapa::spawnPeixes() {
    peixes.clear();

    // Faixas de profundidade (linhas abaixo da superfície) para cada dificuldade
    struct Faixa { Dificuldade dif; unsigned int de, ate, quantidade; };
    const Faixa faixas[] = {
        {Dificuldade::Easy,   1, 3, 4},
        {Dificuldade::Medium, 4, 6, 3},
        {Dificuldade::Hard,   7, 9, 2},
    };

    for (const auto& f : faixas) {
        std::uniform_int_distribution<unsigned int> prof(f.de, f.ate);
        for (unsigned int i = 0; i < f.quantidade; i++) {
            unsigned int y = spawn.y + prof(rng);
            if (y >= current_mapa.size()) continue;

            unsigned int x_min, x_max;
            limitesDaAgua(y, x_min, x_max);
            if (x_max <= x_min) continue;

            std::uniform_int_distribution<unsigned int> col(x_min, x_max);
            peixes.emplace_back(Vector2(col(rng), y), f.dif);
        }
    }
}

void Mapa::fishSwim() {
    for (auto& peixe : peixes) {
        unsigned int x_min, x_max;
        limitesDaAgua(peixe.getPos().y, x_min, x_max);
        peixe.Swim(x_min, x_max);
    }
}

bool Mapa::pescarPeixeEm(Vector2 pos, Peixe& pescado) {
    auto it = std::find_if(peixes.begin(), peixes.end(),
                           [pos](const Peixe& p) { return p.getPos() == pos; });
    if (it == peixes.end()) return false;

    pescado = *it;
    peixes.erase(it);
    return true;
}

unsigned int Mapa::quantidadePeixes() const {
    return peixes.size();
}

void Mapa::printMapa() const {
    std::vector<std::vector<char>> tela = current_mapa;
    bool pescando = (current_ID == "lago");

    // água: '~' só na superfície, o fundo fica limpo para ver os peixes
    for (unsigned int y = 1; pescando && y < tela.size(); y++)
        for (unsigned int x = 0; x < tela[y].size(); x++)
            if (current_mapa[y][x] == '~' && x < current_mapa[y - 1].size() &&
                current_mapa[y - 1][x] == '~')
                tela[y][x] = ' ';

    for (const auto& obj : objetos)
        tela[obj.getPos().y][obj.getPos().x] = obj.getSimbolo();

    for (const auto& peixe : peixes)
        tela[peixe.getPos().y][peixe.getPos().x] = peixe.getSimbolo();

    Vector2 p = player.getPos();
    if (pescando) {
        // linha de pesca descendo da superfície até o anzol
        for (unsigned int y = spawn.y; y < p.y; y++)
            tela[y][p.x] = '|';
        tela[p.y][p.x] = 'J';
    } else {
        tela[p.y][p.x] = '@';
    }

    system("clear");
    for (const auto& linha : tela) {
        for (char c : linha) std::cout << c;
        std::cout << '\n';
    }
    std::cout.flush();
}
