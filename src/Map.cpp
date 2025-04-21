#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>

#include "SDL_render.h"
#include "glm/detail/type_half.hpp"
void Map::AssignRand() {
	if (!Game::overworld) {
		if (Game::level == 1) {
			std::ifstream file("assets/Levels/Level1.txt");
			char type;
			for (int y = 0; y < 72; y++) {
				for (int x = 0; x < 120; x++) {
					file >> type;
					map[x][y] = type;
				}
			}
		}
		if (Game::level == 2) {
			std::ifstream file("assets/Levels/Level2.txt");
			char type;
			for (int y = 0; y < 72; y++) {
				for (int x = 0; x < 120; x++) {
					file >> type;
					map[x][y] = type;
				}
			}
		}
		if (Game::level == 3) {
			std::ifstream file("assets/Levels/Level3.txt");
			char type;
			for (int y = 0; y < 72; y++) {
				for (int x = 0; x < 120; x++) {
					file >> type;
					map[x][y] = type;
				}
			}
		}
	} else {
		std::ifstream file("assets/overworld.txt");
		char type;
		for (int y = 0; y < 72; y++) {
			for (int x = 0; x < 120; x++) {
				file >> type;
				map[x][y] = type;
			}
		}
	}
}
void Map::AssignBorders() {
	if (!Game::overworld) {
		for (int y = 0; y < 72; y++) {
			for (int x = 0; x < 120; x++) {
				if (map[x][y] == '2') {
					if (map[x + 1][y] != '2' || map[x - 1][y] != '2' ||
						map[x][y + 1] != '2' || map[x][y - 1] != '2') {
						Borders.push_back(tile[x][y].dest);
						tile[x][y].isWall = true;
					}
				}

                }
			}
	} else {
		for (int y = 0; y < 72; y++) {
			for (int x = 0; x < 120; x++) {
                if(map[x][y] == '4' || map[x][y] == '7'){
                        DoorTiles.push_back(&tile[x][y]); 
                    }
				if (map[x][y] == '5') {
					if (map[x + 1][y] != '5' || map[x - 1][y] != '5' ||
						map[x][y + 1] != '5' || map[x][y - 1] != '5') {
						OWBorders.push_back(tile[x][y].dest);
						tile[x][y].isWall = true;
					}
				}
				if (map[x][y] == '3') {
					if (map[x + 1][y] != '3' || map[x - 1][y] != '3' ||
						map[x][y + 1] != '3' || map[x][y - 1] != '3') {
						OWBorders.push_back(tile[x][y].dest);
						tile[x][y].isWall = true;
					}
				}
                if(map[x][y] == '8'){
                    if (map[x + 1][y] != '8' || map[x - 1][y] != '8' ||
                            map[x][y + 1] != '8' || map[x][y - 1] != '8') {
                        OWBorders.push_back(tile[x][y].dest);
                        tile[x][y].isWall = true;
                    }
                }


				/*
				if (map[x][y] == '4') {
					if (map[x + 1][y] != '4' || map[x - 1][y] != '4' ||
						map[x][y + 1] != '4' || map[x][y - 1] != '4') {
						OWBorders.push_back(tile[x][y].dest);
						tile[x][y].isWall = true;

					}

				}
				*/
			}
		}
	}
}
void Map::LoadMap() {
	for (int y = 0; y < 72; y++) {
		for (int x = 0; x < 120; x++) {
			tile[x][y].dest.x = x * 32;
			tile[x][y].dest.y = y * 32;
			tile[x][y].dest.w = 32;
			tile[x][y].dest.h = 32;
			tile[x][y].posx = tile[x][y].dest.x;
			tile[x][y].posy = tile[x][y].dest.y;
		}
	}

}
void Map::checkOverWorld() {
	if (!Game::overworld) {
		tile1 = {32, 0, 32, 32};
		tile2 = {32, 64, 32, 32};

	} else {
		tile1 = {0, 271, 32, 32};	 // normal grass
		tile2 = {0, 298, 32, 32};	 // grass z rozicami
		tile3 = {209, 263, 32, 32};	 // gozd
		tile4 = {121, 633, 32, 32};	 // dungeon door
		tile7 = {421, 754, 32, 32};	 // dungeon second door
		tile5 = {211, 720, 32, 32};	 // dungeon wall
		tile6 = {30, 569, 32, 32};	 // dungeon roof
        tile8 = {211,836,32,32}; //center za zivalce walls
        tile9 = {30,809,32,32}; //center za zivalce roof
        tile10 = {121,754,32,32}; //center za zivalce door 
	}
}

void Map::Render() {
	int OffsetX = Game::Camera.x / 32;
	int OffsetY = Game::Camera.y / 32;

	int pixelOffsetX = Game::Camera.x % 32;	 // Sub-tile offset
	int pixelOffsetY = Game::Camera.y % 32;	 // Sub-tile offset

	int maxX = std::min(OffsetX + 61,
						120);  // +1 to prevent gaps at screen edges
	int maxY = std::min(OffsetY + 35, 72);

	for (int y = OffsetY; y < maxY; y++) {
		for (int x = OffsetX; x < maxX; x++) {
			char tileType = map[x][y];

			// Calculate screen position with smooth pixel offset
			SDL_Rect dest = {(x - OffsetX) * 32 - pixelOffsetX,
							 (y - OffsetY) * 32 - pixelOffsetY, 32, 32};

			switch (tileType) {
				case '1':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile1, &dest);
					break;
				case '2':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile2, &dest);
					break;
				case '3':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile3, &dest);
					break;
				case '4':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile4, &dest);
					break;
				case '5':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile5, &dest);
					break;
				case '6':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile6, &dest);
					break;
				case '7':
					SDL_RenderCopy(Game::renderer, Bitmap, &tile7, &dest);
                    break;
                case '8':
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile8, &dest);
                    break;
                case '9':
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile9, &dest);
                    break;
                case '0':
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile10, &dest);
                    break;
				default:
					break;	// Skip empty tiles or unknown types
			}
		}
	}
}
