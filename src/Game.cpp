#include "Game.h"

#include <algorithm>
#include <cfenv>
#include <cmath>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "Bullet.hpp"
#include "GameObject.h"
#include "Map.hpp"
#include "Mouse.hpp"
#include "SDL_cpuinfo.h"
#include "SDL_mutex.h"
#include "SDL_pixels.h"
#include "SDL_platform.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/detail/type_vec4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"
#include "player.hpp"
std::set<Bullet*> bullets;
Player* player;
GameObject* neki;
std::set<Scientist*> scientists;
Mouse mouse;
Map* map;
SDL_Rect Game::Camera = {0, 0, 1920, 1080};

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	int flags = 0;

	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			//	SDL_SetRenderDrawColor(renderer, 100, 100,100, 100);
		}

		isRunning = true;
	}
	srand(time(NULL));
	map = new Map("assets/textures/DungenTileset.png");
	player =new Player("assets/textures/Test2.png", 56*32, 25*32, 48, 48);
	neki = new GameObject("assets/textures/chest.jpg", 300, 300, 32, 32);
	for (int i = 0; i < 1; i++) {
		scientists.insert(new Scientist("assets/textures/scientist.png",
										rand() % (1920), rand() % (1080), 48,
										48));
	}
}

void Game::handleEvents() {
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
	mouse.XY(event);
	mouse.Clicked(event);
}
glm::vec2 FinalMove;
int TimeSinceLastBullet = 1e9;

void Game::update(Clock* ura) {
	player->Update(ura);
   //collision detect for player and tiles:
    /* 
player->CollisionDetect(&map->tile[(int)player->posx/32-1][(int)player->posy/32]);
player->CollisionDetect(&map->tile[(int)player->posx/32][(int)player->posy/32-1]);
player->CollisionDetect(&map->tile[(int)player->posx/32+1][(int)player->posy/32-1]);
player->CollisionDetect(&map->tile[(int)player->posx/32-1][(int)player->posy/32+1]);
player->CollisionDetect(&map->tile[(int)player->posx/32+2][(int)player->posy/32]);
player->CollisionDetect(&map->tile[(int)player->posx/32+2][(int)player->posy/32+1]);
player->CollisionDetect(&map->tile[(int)player->posx/32+1][(int)player->posy/32+2]);
player->CollisionDetect(&map->tile[(int)player->posx/32][(int)player->posy/32+2]);
*/
 for(int x=0;x<120;x++){
       for(int y =0; y<72;y++){
           if(map->tile[x][y].isWall){
               player->CollisionDetect(&map->tile[x][y]);
           }
           else
               continue;
       }
   }
	Camera.x = player->posx - 1920 / 2;
	Camera.y = player->posy - 1080 / 2;
	if (Camera.x < 0) {
		Camera.x = 0;
	}
	if (Camera.y < 0) {
		Camera.y = 0;
	}

	if (Camera.x > Camera.w) {
		Camera.x = Camera.w;
	}

	if (Camera.y >= 1292) {
		Camera.y = 1292;
	}
	neki->Update();
    neki->isWall = true;
	//std::cout << Camera.x << " " << Camera.y << std::endl;
		//std::cout << player->posx << " " << player->posy << std::endl;
        player->CollisionDetect(neki);
	for (Scientist* scientist : scientists) scientist->Update(ura, player);
	if (mouse.click && TimeSinceLastBullet > 750) {
		if (TimeSinceLastBullet > 750) TimeSinceLastBullet = 0;
		Bullet* bullet = new Bullet("assets/textures/bullet.png",
									player->dest.x, player->dest.y, 70, 70);
		bullet->Active = true;
		glm::vec2 vec;
		vec.x = mouse.xpos - player->dest.x;
		vec.y = mouse.ypos - player->dest.y;
		FinalMove = glm::normalize(vec);
		bullet->pos = FinalMove;
		bullets.insert(bullet);
	}
	TimeSinceLastBullet += ura->delta;

	for (Bullet* bullet : bullets) {
		if (bullet != NULL && bullet->Active) {
			bullet->Update(ura);
		}

	}
	std::vector<Bullet*> toDelete;
	std::set<Scientist*> toDeleteScientist;

	for (Bullet* bullet : bullets) {
		for (Scientist* scientist : scientists) {
			if (bullet->CollisionDetect(scientist)) {
				toDeleteScientist.insert(scientist);
				toDelete.push_back(bullet);
			}
		}
		if (!bullet->Active) {
			toDelete.push_back(bullet);
		}
	}

	for (Bullet* bullet : toDelete) {
		bullets.erase(bullet);
		delete bullet;
	}
	for (Scientist* scientist : toDeleteScientist) {
		scientists.erase(scientist);
		delete scientist;
	}
	for (Scientist* scientist : scientists) {
		if (scientist->CollisionDetect(player)) {
			isRunning = false;
		}
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	map->Render();
	player->Render();
	neki->Render();
	for (Scientist* scientist : scientists) scientist->Render();
	for (Bullet* bullet : bullets) {
		if (bullet != NULL && bullet->Active) {
			bullet->Render();
		}
	}

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
