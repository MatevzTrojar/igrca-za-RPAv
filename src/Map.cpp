#include "Map.hpp"
#include <cstdlib>
#include<ctime>

void Map::AssignRand(){
srand(time(NULL));
    for(int  x = 0 ; x < 60; x++){
        for(int y = 0; y < 34; y++){
            map[x][y] = rand() % 4 + 1;
        }
    }
}
void Map::LoadMap(){
    for(int x = 0; x < 60; x++){
        for(int y = 0; y < 34; y++){
            tile[x][y].x = x * 32;
            tile[x][y].y = y * 32;
            tile[x][y].w = 32;
            tile[x][y].h = 32;
        }
    }
}
void Map::Render(){
    for(int x = 0;x<60;x++){
        for(int y = 0;y<34;y++){
            switch(map[x][y]){
                case 1:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile1, &tile[x][y]);           
                    break;
                case 2:
                    SDL_RenderCopy(Game::renderer,Beatmap, &tile2, &tile[x][y]);           
                    break;
                case 3:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile3, &tile[x][y]);           
                    break;
                case 4:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile4, &tile[x][y]);           
                    break;
            }
        }
    }

}
