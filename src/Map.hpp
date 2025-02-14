#include <immintrin.h>
#include <cmath>
#include "SDL.h"
#include "TextureManager.h"
class Map {
public:
    SDL_Texture* Beatmap;
    int offsetX=0,offsetY=0;
    int map[120][72];
    SDL_Rect tile[120][72];
    SDL_Rect tile1 = {0,0,32,32};
    SDL_Rect tile2 = {32,0,32,32};
    SDL_Rect tile3 = {0,32,32,32};
    SDL_Rect tile4 = {32,32,32,32};
    SDL_Rect tile5 = {0,64,32,32};
    SDL_Rect tile6 = {32,64,32,32};
    SDL_Rect tile7 = {0,96,32,32};
    SDL_Rect tile8 = {32,96,32,32};
    SDL_Rect tile9 = {64,0,32,32};
    SDL_Rect tile10 = {96,0,32,32};
    SDL_Rect tile11 = {64,32,32,32};
    SDL_Rect tile12 = {96,32,32,32};
    SDL_Rect tile13 = {64,64,32,32};
    SDL_Rect tile14 = {96,64,32,32};
    SDL_Rect tile15 = {64,96,32,32};
    SDL_Rect tile16 = {96,96,32,32};

    Map(const char *a){
       Beatmap = TextureManager::LoadTexture(a);
    AssignRand();
    LoadMap();
    };
    void AssignRand();
    void LoadMap();
    void Render();

};

