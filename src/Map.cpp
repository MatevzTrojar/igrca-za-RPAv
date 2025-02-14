#include "Map.hpp"
#include <cstdlib>
#include<ctime>

void Map::AssignRand(){
srand(time(NULL));
    for(int  x = 0 ; x < 120; x++){
        for(int y = 0; y < 72; y++){
            map[x][y] = rand() % 16 + 1;
        }
    }
}
void Map::LoadMap(){
    for(int x = 0; x < 120; x++){
        for(int y = 0; y < 72; y++){
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
            if( offsetX >= 90 || offsetX<=-30){
                continue;
            }
            switch(map[x+offsetX][y+offsetY]){
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
                case 5:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile5, &tile[x][y]);           
                    break;
                case 6:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile6, &tile[x][y]);           
                    break;
                case 7:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile7, &tile[x][y]);           
                    break;
                case 8:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile8, &tile[x][y]);           
                    break;
                case 9:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile9, &tile[x][y]);           
                    break;
                case 10:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile10, &tile[x][y]);           
                    break;
                case 11:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile11, &tile[x][y]);           
                    break;
                case 12:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile12, &tile[x][y]);           
                    break;
                case 13:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile13, &tile[x][y]);           
                    break;
                case 14:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile14, &tile[x][y]);           
                    break;
                case 15:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile15, &tile[x][y]);           
                    break;
                case 16:
                    SDL_RenderCopy(Game::renderer, Beatmap, &tile16, &tile[x][y]);           
                    break;
            }
        }
    }

}
