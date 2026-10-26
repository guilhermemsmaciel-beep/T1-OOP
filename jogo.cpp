#include "jogo.h"
#include <iostream>
#include <fstream>
#include "json.hpp"


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

using json = nlohmann::json;

Jogo::Jogo(Mapa* map, Pescador& p) : player(p), mapa(map) 
{
    //Left blank on purpose
    screen = mapa;
}

void Jogo::addObject(Object obj) {
    objetos.push_back(obj);
    //objs.insert(std::pair<Mapa*, std::vector<Object>>(mapa, objetos));
}

int Jogo::getObjetosSize() {
    return objetos.size();
}

void Jogo::DesenharObj() { 
    try{
        for(int i = 0; i < objetos.size(); i++) {
            objetos.at(i).Desenhar();
        }
    } catch(std::out_of_range& e) {
        std::cout << "Erro capturado de forma segura" << e.what() << std::endl;
    }
}

void Jogo::SetMapa(std::string caminho_arquivo) {
    Mapa* leste = new Mapa("leste", WINDOW_X, WINDOW_Y, BLUE);
    Mapa* oeste = new Mapa("oeste", WINDOW_X, WINDOW_Y, BLUE);
    Mapa* norte = new Mapa("norte", WINDOW_X, WINDOW_Y, BLUE);
    Mapa* sul = new Mapa("sul", WINDOW_X, WINDOW_Y, BLUE);
    Mapa* nordeste = new Mapa("nordeste", WINDOW_X, WINDOW_Y, BLUE);
    Mapa* noroeste = new Mapa("noroeste", WINDOW_X, WINDOW_Y, BLUE);
    Mapa* sudeste = new Mapa("sudeste", WINDOW_X, WINDOW_Y, BLUE);
    Mapa* sudoeste = new Mapa("sudoeste", WINDOW_X, WINDOW_Y, BLUE);

    mapa->setDirections(oeste, leste, norte, sul);
    leste->setDirections(mapa, nullptr, nordeste, sudeste);
    nordeste->setDirections(norte, nullptr, nullptr, leste);
    norte->setDirections(noroeste, nordeste, nullptr, mapa);
    noroeste->setDirections(nullptr, norte, nullptr, oeste);
    oeste->setDirections(nullptr, mapa, noroeste, sudoeste);
    sudoeste->setDirections(nullptr, sul, oeste, nullptr);
    sul->setDirections(sudoeste, sudeste, mapa, nullptr);
    sudeste->setDirections(sul, nullptr, leste, nullptr);

    std::ifstream arquivo(caminho_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo" << caminho_arquivo << std::endl;

    }
    json dados_json;
    arquivo >> dados_json;
    loaded_objs = dados_json;
}

void Jogo::LoadMapa() {
    auto it = loaded_objs.find(mapa->Address());

    if (it != loaded_objs.end()) {
        screen_objs = loaded_objs.at(mapa->Address());
    }

    for(int i = 0; i < screen_objs.size(); i++) {
        Object obj(screen_objs[i][0], screen_objs[i][1], OBJ_X, OBJ_Y);
        objetos.push_back(obj);
    }
    
}

void Jogo::UnloadMapa() {
    screen_objs.clear();
    objetos.clear();
}

    // for(auto it = objetos.begin(); it != objetos.end(); it++) {
    //     Object tmp(it[0], it[1], OBJ_X, OBJ_Y);
    //     aux.push_back(tmp);
    // }
    // objs.insert(std::pair<Mapa*, std::vector<Object>>(mapa, aux));


void Jogo::changeMap(EDirection direcao) {
    if (direcao == EDirection::Left && mapa->Left() != nullptr) {
        mapa = mapa->Left();
        screen = mapa;
        screen_objs.clear();
        objetos.clear();
        LoadMapa();
        //std::cout << "Map change is working" << std::endl;
        player.setPosicaoX(mapa->sizeOf().x);
    }
    if (direcao == EDirection::Right && mapa->Right() != nullptr) {
        
        mapa = mapa->Right();
        screen = mapa;
        screen_objs.clear();
        objetos.clear();
        LoadMapa();
        //std::cout << "Map change is working" << std::endl;
        player.setPosicaoX(0.0);
    }
    if (direcao == EDirection::Up && mapa->Up() != nullptr) {
        mapa = mapa->Up();
        screen = mapa;
        screen_objs.clear();
        objetos.clear();
        LoadMapa();
        //std::cout << "Map change is working" << std::endl;
        player.setPosicaoY(mapa->sizeOf().y);
    }
    if (direcao == EDirection::Down && mapa->Down() != nullptr) {
        mapa = mapa->Down();
        screen = mapa;
        screen_objs.clear();
        objetos.clear();
        LoadMapa();
        //std::cout << "Map change is working" << std::endl;
        player.setPosicaoY(0.0);
    }

    std::cout << "Tam: " << getObjetosSize() << std::endl;
    
}

void Jogo::fishingGame() {
    Peixe fish(-5.0f, 300.0f, 20.0f, 5.0f);
    Vector2 pos = player.getPosicao();
    player.setPosicao({50, 100});
    while(!IsKeyPressed(KEY_ENTER) && !WindowShouldClose()) {
        
        player.LerEntrada({WINDOW_X, WINDOW_Y});
        fish.Swim();

        BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawTexture(player.getTexture(), player.getPosicao().x, player.getPosicao().y, WHITE);
            fish.Desenhar();
        EndDrawing();
    }
    player.setPosicao({pos.x + 50, pos.y});
    
}

void Jogo::run_game() {
    InitWindow(mapa->sizeOf().x, mapa->sizeOf().y, "Pescaria - Teste Raylib");
    SetTargetFPS(60);

    LoadMapa();
    player.Desenhar();
    Rectangle sourceRec = {0.0f, 0.0f, (float)player.getTexture().width, (float)player.getTexture().height};
    Vector2 origin = {0.0f, 0.0f};

    while(!IsKeyPressed(KEY_ENTER) && !WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(YELLOW);
            DrawRectangle(RECT_CENTRO_X, RECT_CENTRO_Y, 
                          RECT_SIZE_X, RECT_SIZE_Y, RED);

            DrawText("Pescaria", TITLE_CENTRO_X, TITLE_CENTRO_Y, 
                      TITLE_FONTSIZE, BLACK);

            DrawText("Press start to begin", SUB_CENTRO_X, 
                      SUB_CENTRO_Y, SUB_FONTSIZE, BLACK);
        EndDrawing();
    }
    
    while (!WindowShouldClose()) {

        this->player.LerEntrada(*mapa);
        try {
            for(auto it = objetos.begin(); it != objetos.end(); it++) {
                if(player.Interagiu(*it)) 
                    fishingGame();
            }
        } catch (std::out_of_range& e) {
            std::cout << "Erro capturado" << e.what() << std::endl;
        }
        // for(auto it = screen->mapBegin(); it != screen->mapEnd(); it++) {
        //     if(this->player.Interagiu(*it)) {
        //         while(!IsKeyPressed(KEY_ENTER) && !WindowShouldClose()) {
        //             BeginDrawing();
        //                 ClearBackground(PINK);
        //             EndDrawing();
        //         }
        //         float x = player.getPosicao().x;
        //         player.setPosicaoX(x+50);
        //     }
        // }
        EDirection change = this->player.Atualizar(mapa);
        changeMap(change);
        
        if (this->player.isAlive()) {
            BeginDrawing();
            
            ClearBackground(mapa->getCor());
                DesenharObj();
                //mapa->Desenhar(); //erro nessa linha
                DrawTexturePro(player.getTexture(), sourceRec, player.getRec(), origin, 0.0f, WHITE);
                //this->player.Desenhar();
            EndDrawing();
        } 
        else {
            //mapa->Desenhar();
            DesenharObj();
            DrawTexturePro(player.getTexture(), sourceRec, player.getRec(), origin, 0.0f, WHITE);
            //this->player.Desenhar();
            // sleep(1);
            break;
        }
    }
    CloseWindow();
}


//rightmost limit = map limit - player size / 2
// leftmost limit = 0;
//upmost limit = 0
//downmost limit = map limit - player size / 2

int main() {

    Pescador jogador(400.0f, 300.0f, 70.0f, 70.0f);
    //Object obj(500.0f, 400.0f, 40.0f, 40.0f);
    Mapa* mapa = new Mapa("centro", WINDOW_X, WINDOW_Y, BLUE);
    //mapa->addObject(obj);
    //mapa->setRight();
    //mapa->setDirections();
    //mapa->addObject(obj);

    Jogo pesca(mapa, jogador);
    pesca.SetMapa("map_objects.json");
    //pesca.addObject(obj);

    pesca.run_game();

    return 0;
}


//g++ -std=c++17 $(find . -name "*.cpp") -o teste $(pkg-config --libs --cflags raylib)


