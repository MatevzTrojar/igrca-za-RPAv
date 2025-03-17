#include <immintrin.h>
#include <cmath>
#include "SDL.h"
#include "SDL_rect.h"
#include "TextureManager.h"
#include "Tile.hpp"
#include<vector>
class Map {
public:
    std::vector<SDL_Rect> Borders;
    std::vector<SDL_Rect> OWBorders;
    SDL_Texture* Bitmap;
    char map[120][72];
     Tile tile[120][72];
     /*
    SDL_Rect tile1 = {0,0,32,32};
    SDL_Rect tile2 = {128,0,32,32};
    */
     SDL_Rect tile1;
    SDL_Rect tile2;
    SDL_Rect tile3;
    SDL_Rect tile4;
    SDL_Rect tile5;
    SDL_Rect tile6;

    Map(const char *a){
       Bitmap = TextureManager::LoadTexture(a);

    checkOverWorld();
    AssignRand();
    LoadMap();
    AssignBorders();
    };
    void AssignRand();
    void LoadMap();
    void Render();
    void AssignBorders();
    void checkOverWorld();

};

