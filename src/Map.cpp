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
    int OffsetX = Game::Camera.x / 32;
    int OffsetY = Game::Camera.y / 32;
    for(int x = 0;x<60;x++){
        for(int y = 0;y<34;y++){
                 switch(map[x+OffsetX][y+OffsetY]){ 
                case 1:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile1, &tile[x][y]);           
                    break;
                case 2:
                    SDL_RenderCopy(Game::renderer,Bitmap, &tile2, &tile[x][y]);           
                    break;
                case 3:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile3, &tile[x][y]);           
                    break;
                case 4:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile4, &tile[x][y]);           
                    break;
                case 5:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile5, &tile[x][y]);           
                    break;
                case 6:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile6, &tile[x][y]);           
                    break;
                case 7:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile7, &tile[x][y]);           
                    break;
                case 8:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile8, &tile[x][y]);           
                    break;
                case 9:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile9, &tile[x][y]);           
                    break;
                case 10:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile10, &tile[x][y]);           
                    break;
                case 11:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile11, &tile[x][y]);           
                    break;
                case 12:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile12, &tile[x][y]);           
                    break;
                case 13:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile13, &tile[x][y]);           
                    break;
                case 14:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile14, &tile[x][y]);           
                    break;
                case 15:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile15, &tile[x][y]);           
                    break;
                case 16:
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile16, &tile[x][y]);           
                    break;
            }
        }
    }

}
