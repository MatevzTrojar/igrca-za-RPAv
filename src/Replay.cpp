#include "Replay.hpp"

#include <ctime>
#include <exception>
#include <experimental/filesystem>
#include <fstream>
#include <utility>

#include "Collision.hpp"
#include "Game.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "TextureManager.h"
#include "glm/detail/type_half.hpp"
std::vector<Game::DeltaPlayer> Replay::replayPath;
Game::ReplayData ReplayData;
size_t Replay::currentFrame = 0;
void Replay::initReplay(Game* game) {
	std::cout << "Replay::initReplay called" << std::endl;
	replayPath.clear();
	currentFrame = 0;
	std::ifstream replayFile("replay.bin", std::ios::binary);
	if (!replayFile) {
		std::cerr << "Error opening replay file." << std::endl;
		return;
	}
	replayFile.read(reinterpret_cast<char*>(&ReplayData),
					sizeof(Game::ReplayData));
	Game::DeltaPlayer point;
	while (replayFile.read(reinterpret_cast<char*>(&point), sizeof(point))) {
		replayPath.push_back(point);
	}

	Game::LeftDungeon1 = ReplayData.LeftDungeon1;
	Game::LeftDungeon2 = ReplayData.LeftDungeon2;
	Game::LeftDungeon3 = ReplayData.LeftDungeon3;
	std::cout << "ReplayData.LeftDungeon1.x/y: " << ReplayData.LeftDungeon1.x
			  << ReplayData.LeftDungeon1.y << std::endl;
	replayFile.close();
	game->Overworldinit(true);
}

static Uint32 lastReplayTick = 0;

void Replay::Update(Game* game) {
	if (currentFrame < replayPath.size()) {

            std::cout<<Game::level<<std::endl;
        for(SDL_Rect Border : Game::map->Borders){
            Game::player->CollisionDetect(Border);
        }
		// Update player position from replay
		Game::player->posx = replayPath[currentFrame].posX;
		Game::player->posy = replayPath[currentFrame].posY;
		Game::player->dest.x =
			static_cast<int>(Game::player->posx) - game->Camera.x;
		Game::player->dest.y =
			static_cast<int>(Game::player->posy) - game->Camera.y;

		for (int i = 0; i < Game::map->DoorTiles.size(); i++) {
			SDL_Rect TileDest{(int)Game::map->DoorTiles[i]->posx,
							  (int)Game::map->DoorTiles[i]->posy,
							  Game::map->DoorTiles[i]->dest.w,
							  Game::map->DoorTiles[i]->dest.h};
			SDL_Rect TempPlayer{(int)Game::player->posx,
								(int)Game::player->posy, Game::player->dest.w,
								Game::player->dest.h};
			if (Collision::AABB(TileDest, TempPlayer) &&
				!Game::map->DoorTiles[i]->Used && game->overworld) {
				Game::map->DoorTiles[i]->Used = true;

				std::cout << "Col detect:" << std::endl;
				Game::overworld = false;
				/*
				Game::map->Bitmap =
					TextureManager::LoadTexture("assets/textures/Dungeon.png");
				Game::map->checkOverWorld();
				Game::map->AssignRand();
				Game::map->LoadMap();
				Game::map->AssignBorders();
				return;
				*/

				game->level++;
				game->Dungeoninit(1);
			}
		}

		if (game->level == 1 && !game->overworld &&
			game->LeftDungeon1.x == static_cast<int>(Game::player->posx) &&
			game->LeftDungeon1.y == static_cast<int>(Game::player->posy)) {
			game->overworld = true;
			 game->Overworldinit(1);
		} else if (game->level == 2 && !game->overworld &&
				   game->LeftDungeon2.x ==
					   static_cast<int>(Game::player->posx) &&
				   game->LeftDungeon2.y ==
					   static_cast<int>(Game::player->posy)) {
            Game::overworld = true;
			game->Overworldinit(1);
		} else if (game->level == 3 && !game->overworld &&
				   game->LeftDungeon3.x ==
					   static_cast<int>(Game::player->posx) &&
				   game->LeftDungeon3.y ==
					   static_cast<int>(Game::player->posy)) {
			game->overworld = true;
			game->Overworldinit(1);
		}

		// Frame limiter for replay (60 fps)
		Uint32 now = SDL_GetTicks();
		if (now - lastReplayTick >= 1000 / 60) {
			currentFrame++;
			lastReplayTick = now;
		}
	} else {
		game->playingReplay = false;
		game->isRunning = false;
	}
}

void Replay::render(Game* game) {
	SDL_RenderClear(game->renderer);
	Game::player->Render();
	Game::map->Render();
	SDL_RenderPresent(game->renderer);
}
