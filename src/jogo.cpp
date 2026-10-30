#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

#include "jogo.h"
#include "json.hpp"
#include "edificuldade.h"


// Default window size
#define WINDOW_X 800.0
#define WINDOW_Y 600.0

// Default object size
#define OBJ_X 40.0
#define OBJ_Y 40.0

// TITLE SCREEN
#define TITLE_CENTRO_X 325
#define TITLE_CENTRO_Y 250
#define TITLE_FONTSIZE 40

#define SUB_CENTRO_X 250
#define SUB_CENTRO_Y 300
#define SUB_FONTSIZE 30

#define RECT_CENTRO_X 235
#define RECT_CENTRO_Y 250
#define RECT_SIZE_X 350
#define RECT_SIZE_Y 100

//using json = nlohmann::json;

// Jogo::Jogo(Mapa* map, Pescador& p) : player(p), mapa(map) 
// {
//     //Left blank on purpose
//     screen = mapa;
// }



// void Jogo::addObject(Object obj) {
//     objetos.push_back(obj);
//     //objs.insert(std::pair<Mapa*, std::vector<Object>>(mapa, objetos));
// }

// int Jogo::getObjetosSize() {
//     return objetos.size();
// }








    // for(auto it = objetos.begin(); it != objetos.end(); it++) {
    //     Object tmp(it[0], it[1], OBJ_X, OBJ_Y);
    //     aux.push_back(tmp);
    // }
    // objs.insert(std::pair<Mapa*, std::vector<Object>>(mapa, aux));




void Jogo::game_preparation() {
    screen = new Mapa(player);
    mapas.push_back("mapa.txt");
    mapas.push_back("fishing_map.txt");
}


void Jogo::game_intro() {

    std::string nome;
    char answer;

    std::cout << "Welcome to Fishz!\nPlease name your character:\n";    
    std::cin >> nome;
    std::cout << "Are you sure of the name " << nome << "?" << std::endl;

    std::cin >> answer;
    if (answer == 'N' || answer == 'n') {
        std::cout << "Please type your name: " << std::endl;
        std::cin >> nome;
    }
    player.setNome(nome);

    std::cout << "Alright " << player.getNome() << ", let's get you started. First, your most important tool, your fishing rod!" <<std::endl;
    std::cout << "Here, take this. It's not the fanciest, but it'll do the job with smaller fish" << std::endl;

    Rod *rod = new Rod();
    rod->itemDescription();
    player.addRod(rod);

    std::cout << "I just added it to your inventory. If you want you can check it, just press I" << std::endl;

    std::cin >> answer;
    if (answer == 'I' || answer == 'i') {
        player.printInventario();
        std::cout << "Yes, that's it! Ok, now i will teach how to fish. Let's go to the pond" << std::endl;
    } else {
        sleep(5);
        std::cout << "Anyways. Now, let's get to the point. I'll teach you how to catch fish, let's go to the pond" << std::endl;
    }


    player.setPos(19, 3);
    screen->loadMapas(mapas);
    screen->loadMapa();
    screen->printMapa();
    try {
        while (player.getPos() != screen->getObjectPos(0)) {
            system("stty raw");
            char comando = std::getchar();
            if (comando == 'e') {
                break;
            }
            system("stty cooked");
            player.atualizarPos(comando, screen->getMapa());
            screen->printMapa();

        }
    } catch (std::out_of_range& e) {
        std::cerr << "Erro no loop de print" << e.what() << std::endl;
    }
    
    system("stty sane");
    std::cout << player.getPosX() << ", " << player.getPosY() << std::endl;
    std::cout << screen->getObjectPos(0).x << ", " << screen->getObjectPos(0).y << std::endl;
    //system("stty cooked");
    


}


void Jogo::run_fishing_game() {
    screen->fishingGameTest();
    player.setPos(10, 4);
    while (1) {
        system("stty raw");
        char comando = std::getchar();
        if (comando == 'e') {
            break;
        }
        system("stty cooked");
        screen->peixesSwim();
        screen->printMapa();
        player.atualizarPos(comando, screen->getMapa(), 'y');
        
    }
    system("stty sane");

}


void Jogo::run_game() {

}


//rightmost limit = map limit - player size / 2
// leftmost limit = 0;
//upmost limit = 0
//downmost limit = map limit - player size / 2

int main() {

    // Pescador jogador(400.0f, 300.0f, 70.0f, 70.0f);
    // //Object obj(500.0f, 400.0f, 40.0f, 40.0f);
    // Mapa* mapa = new Mapa("centro", WINDOW_X, WINDOW_Y, BLUE);
    // //mapa->addObject(obj);
    // //mapa->setRight();
    // //mapa->setDirections();
    // //mapa->addObject(obj);

    // Jogo pesca(mapa, jogador);
    // pesca.SetMapa("map_objects.json");
    // //pesca.addObject(obj);

    Pescador player;
    Jogo fishz(player);

    fishz.game_preparation();
    fishz.game_intro();
    fishz.run_fishing_game();


    return 0;
}


//g++ -Iinclude pescador.cpp jogo.cpp -o teste


