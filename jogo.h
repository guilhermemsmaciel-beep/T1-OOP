#ifndef JOGO_H
#define JOGO_H

#include <vector>
#include <string>
#include <map>

#include "mapa.h"
#include "pescador.h"
#include "edirection.h"
#include "object.h"

class Jogo {
private:
    Mapa* screen;
    std::map<std::string, std::vector<std::array<int, 2>>> loaded_objs;
    //std::map<Mapa*, std::vector<Object>> objs;  
    std::vector<std::array<int, 2>> screen_objs;
    std::vector<Object> objetos;
    Mapa* mapa;
    Pescador& player;
    //Tempo

public:
    Jogo(Mapa* map, Pescador& p);
    void addObject(Object obj);
    void run_game();
    void changeMap(EDirection direcao);
    void fishingGame();
    void DesenharObj();
    void SetMapa(std::string caminho_arquivo);
    void LoadMapa();
    void UnloadMapa();
    int getObjetosSize();
};

#endif
