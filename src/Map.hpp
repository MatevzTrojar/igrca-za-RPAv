#include <immintrin.h>
#include <cmath>
#include "SDL.h"
#include "TextureManager.h"
class Map {
public:
    SDL_Texture* Beatmap;
    int offsetX=0,offsetY=0;
    int map[120][34];
    SDL_Rect tile[60][34];
    SDL_Rect tile1 = {0,0,32,32};
    SDL_Rect tile2 = {32,0,32,32};
    SDL_Rect tile3 = {0,32,32,32};
    SDL_Rect tile4 = {32,32,32,32};


    Map(const char *a){
       Beatmap = TextureManager::LoadTexture(a);
    AssignRand();
    LoadMap();
    };
    void AssignRand();
    void LoadMap();
    void Render();

};

