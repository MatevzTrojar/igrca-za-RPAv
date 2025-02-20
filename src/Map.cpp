#include "Map.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
void Map::AssignRand() {
	srand(time(NULL));
	std::ifstream file("assets/Map.txt");
	char type;
	for (int y = 0; y < 72; y++) {
		for (int x = 0; x < 120; x++) {
			file >> type;
			map[x][y] = type;
		}
	}

	/*    for(int  x = 0 ; x < 120; x++){
			for(int y = 0; y < 72; y++){
				map[x][y] = 2;
			}
		}
	}*/
}
void Map::LoadMap() {
	for (int y = 0; y < 72; y++) {
		for (int x = 0; x < 72; x++) {
			tile[x][y].dest.x = x * 32;
			tile[x][y].dest.y = y * 32;
			tile[x][y].dest.w = 32;
			tile[x][y].dest.h = 32;
            tile[x][y].posx = tile[x][y].dest.x;
            tile[x][y].posy = tile[x][y].dest.y;
		}
	}
}
void Map::Render() {
	int OffsetX = Game::Camera.x / 32;
	int OffsetY = Game::Camera.y / 32;
	//std::cout << OffsetX << " " << OffsetY << std::endl;
		for (int y = 0; y < 34; y++)	{		// screen size
for (int x = 0; x < 60; x++)  {	// screen width

		/*	if (tile[x][y].IsOffScreen()) {
				tile[x][y].isRendered = false;
				break;
			}*/
			switch (map[x + OffsetX][y + OffsetY]) {
				case '1':
                    tile[x][y].isWall = false;
					SDL_RenderCopy(Game::renderer, Bitmap, &tile1,
								   &tile[x][y].dest);
					break;
				case '2':
                    tile[x][y].isWall = true;
					SDL_RenderCopy(Game::renderer, Bitmap, &tile2,
								   &tile[x][y].dest);
					break;
					/*
				case '3':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile3, &tile[x][y]);
					break;
				case '4':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile4, &tile[x][y]);
					break;
				case '5':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile5, &tile[x][y]);
					break;
				case '6':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile6, &tile[x][y]);
					break;
				case '7':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile7, &tile[x][y]);
					break;
				case '8':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile8, &tile[x][y]);
					break;
				case '9':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile9, &tile[x][y]);
					break;
				case '10':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile10,
				&tile[x][y]); break; case '11': SDL_RenderCopy(Game::renderer,
				Bitmap, &tile11, &tile[x][y]); break; case '12':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile12,
				&tile[x][y]); break; case '13': SDL_RenderCopy(Game::renderer,
				Bitmap, &tile13, &tile[x][y]); break; case '14':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile14,
				&tile[x][y]); break; case '15': SDL_RenderCopy(Game::renderer,
				Bitmap, &tile15, &tile[x][y]); break; case '16':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile16,
				&tile[x][y]); break;
					  */
					/*

								   case 1:
									   tile[x][y].isRendered = true;
									   tile[x][y].isWall = false;

									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile1, &tile[x][y].tile); break; case 2:
									   tile[x][y].isWall = true;
									   SDL_RenderCopy(Game::renderer,Bitmap,
					   &tile2, &tile[x][y].tile); tile[x][y].isRendered = true;
									   break;
													  case 3:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile3, &tile[x][y]); break; case 4:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile4, &tile[x][y]); break; case 5:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile5, &tile[x][y]); break; case 6:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile6, &tile[x][y]); break; case 7:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile7, &tile[x][y]); break; case 8:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile8, &tile[x][y]); break; case 9:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile9, &tile[x][y]); break; case 10:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile10, &tile[x][y]); break; case 11:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile11, &tile[x][y]); break; case 12:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile12, &tile[x][y]); break; case 13:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile13, &tile[x][y]); break; case 14:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile14, &tile[x][y]); break; case 15:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile15, &tile[x][y]); break; case 16:
									   SDL_RenderCopy(Game::renderer, Bitmap,
					   &tile16, &tile[x][y]); break;
									   */
			}
		}
	}

}
