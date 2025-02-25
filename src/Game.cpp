#include "Game.h"

#include <algorithm>
#include <cfenv>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <ios>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "Bullet.hpp"
#include "Collision.hpp"
#include "GameObject.h"
#include "Map.hpp"
#include "Mouse.hpp"
#include "SDL_audio.h"
#include "SDL_rect.h"
#include "SDL_render.h"
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
	player = new Player("assets/textures/Test2.png", 56 * 32, 25 * 32, 48, 48);
	neki = new GameObject("assets/textures/chest.jpg", 300, 300, 48, 48);
	for (int i = 0; i < 1; i++) {
		scientists.insert(
			new Scientist("assets/textures/scientist.png", 400, 400, 32, 32));
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
int TimeSinceLastBullet = 1e9;

void Game::update(Clock* ura) {
	// playe movement
	player->Update(ura);
	// Border collision(player,scientist)
	for (SDL_Rect Border : map->Borders) {
		player->CheckCollisionSide(Border);
		for (Scientist* scientist : scientists) {
			scientist->CheckCollisionSide(Border);
		}
	}
	// kamera
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
	player->CheckCollisionSide(neki->dest);
	// Scientist movement
	for (Scientist* scientist : scientists) scientist->Update(ura, player);
	// bullet init
	if (mouse.click && TimeSinceLastBullet > 750) {
		if (TimeSinceLastBullet > 750) TimeSinceLastBullet = 0;
		Bullet* bullet = new Bullet("assets/textures/bullet.png", player->posx,
									player->posy, 70, 70);
		bullet->Active = true;
		bullet->pos.x = mouse.xpos + Camera.x - player->posx;
		bullet->pos.y = mouse.ypos + Camera.y - player->posy;
		bullet->pos = glm::normalize(bullet->pos);
		bullets.insert(bullet);
	}
	TimeSinceLastBullet += ura->delta;

	// bullet movement
	for (Bullet* bullet : bullets) {
		if (bullet != NULL && bullet->Active) {
			bullet->Update(ura,player);
		}
	} 	

    for(Bullet* bullet : bullets){
        for(SDL_Rect Border : map->Borders){
            SDL_Rect tempBullet{(int)bullet->posx, (int)bullet->posy, bullet->dest.w, bullet->dest.h};
            if(Collision::AABB(tempBullet,Border)){
                bullet->Active = false;
            }
        }
    }
		 		 
	// bullet collision
	std::vector<Bullet*> toDelete;
	std::set<Scientist*> toDeleteScientist;
	for (Bullet* bullet : bullets) {
		for (Scientist* scientist : scientists) {
			if (Collision::AABB(bullet->dest, scientist->dest)) {
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
		SDL_Rect tempPlayer{(int)player->posx, (int)player->posy,
							player->dest.w, player->dest.h};
		SDL_Rect tempScientist{(int)scientist->posx, (int)scientist->posy,
							   scientist->dest.w, scientist->dest.h};
		if (Collision::AABB(tempPlayer, tempScientist)) {
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
