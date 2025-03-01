#include "Map.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
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
void Map::AssignBorders() {
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

void Map::Render() {
    int OffsetX = Game::Camera.x / 32;
    int OffsetY = Game::Camera.y / 32;

    int pixelOffsetX = Game::Camera.x % 32; // Sub-tile offset
    int pixelOffsetY = Game::Camera.y % 32; // Sub-tile offset

    int maxX = std::min(OffsetX + 61, 120); // +1 to prevent gaps at screen edges
    int maxY = std::min(OffsetY + 35, 72);

    for (int y = OffsetY; y < maxY; y++) {
        for (int x = OffsetX; x < maxX; x++) {
            char tileType = map[x][y];

            // Calculate screen position with smooth pixel offset
            SDL_Rect dest = { 
                (x - OffsetX) * 32 - pixelOffsetX, 
                (y - OffsetY) * 32 - pixelOffsetY, 
                32, 32 
            };

            switch (tileType) {
                case '1':
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile1, &dest);
                    break;
                case '2':
                    SDL_RenderCopy(Game::renderer, Bitmap, &tile2, &dest);
                    break;
                default:
                    break; // Skip empty tiles or unknown types
            }
        }
    }
}


