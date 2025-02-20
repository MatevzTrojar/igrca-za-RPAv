#include <immintrin.h>
#include <cmath>
#include "SDL.h"
#include "TextureManager.h"
#include "Tile.hpp"
class Map {
public:
    SDL_Texture* Bitmap;
    char map[120][72];
     Tile tile[120][72];
    SDL_Rect tile1 = {0,0,32,32};
    SDL_Rect tile2 = {128,0,32,32};


    Map(const char *a){
       Bitmap = TextureManager::LoadTexture(a);
    AssignRand();
    LoadMap();
    };
    void AssignRand();
    void LoadMap();
    void Render();

};

