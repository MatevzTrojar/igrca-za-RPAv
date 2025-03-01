#include "Game.h"

#include <algorithm>
#include <cfenv>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "Bullet.hpp"
#include "Collision.hpp"
#include "GameObject.h"
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
int life = 3;
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
	map = new Map("assets/textures/Dungeon.png");
	player = new Player("assets/textures/Test2.png", 56 * 32, 25 * 32, 64, 64);
	neki = new GameObject("assets/textures/chest.jpg", 300, 300, 48, 48);
	std::vector<std::vector<int>> RoomSpawn;
	RoomSpawn.push_back({400, 300});							 // room1
	RoomSpawn.push_back({42 * 32, 30 * 32, 72 * 32, 14 * 32});	 // room2
	RoomSpawn.push_back({72 * 32, 43 * 32, 102 * 32, 50 * 32});	 // room3
	RoomSpawn.push_back({111 * 32, 11 * 32});					 // room4
	RoomSpawn.push_back({5 * 32, 53 * 32, 28 * 32, 70 * 32});	 // room5
	int ScientistX;
	int ScientistY;
	for (std::vector<int> i : RoomSpawn) {
		int counter = 0;
		for (int j : i) {
			if (counter % 2 == 0) {
				ScientistX = j;
			} else {
				ScientistY = j;
			}
			counter++;
			if (counter == 2) {
				scientists.insert(new Scientist("assets/textures/scientist.png",
												ScientistX, ScientistY, 64,
												64));
				counter = 0;
			}
		}
	}
}
/*
for (int i = 0; i < 1; i++) {
	scientists.insert(
		new Scientist("assets/textures/scientist.png", 370, 300, 64, 64));
}
*/

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
float collisionCooldown =0;	// Cooldown timer for collision-based life loss (in seconds)
void Game::update(Clock* ura) {
	// playe movement
	player->Update(ura);
	// Border collision(player,scientist)
	for (SDL_Rect Border : map->Borders) {
		player->CollisionDetect(Border);
		for (Scientist* scientist : scientists) {
			scientist->CollisionDetect(Border);
		}
	}
	// kamera

	neki->Update();
	player->CollisionDetect(neki->dest);
	// Scientist movement
	for (Scientist* scientist : scientists) scientist->Update(ura, player, map);
	// bullet init
	if (mouse.click && TimeSinceLastBullet > 750) {
		if (TimeSinceLastBullet > 750) TimeSinceLastBullet = 0;
		Bullet* bullet =
			new Bullet("assets/textures/bullet.png", player->posx + 10,
					   player->posy + 10, 48, 48);
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
			bullet->Update(ura, player);
		}
	}

	for (Bullet* bullet : bullets) {
		for (SDL_Rect Border : map->Borders) {
			SDL_Rect tempBullet{(int)bullet->posx, (int)bullet->posy,
								bullet->dest.w, bullet->dest.h};
			if (Collision::AABB(tempBullet, Border)) {
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

    
	collisionCooldown -= ura->delta;
	for (Scientist* scientist : scientists) {
		SDL_Rect tempPlayer{(int)player->posx, (int)player->posy,
							player->dest.w, player->dest.h};
		SDL_Rect tempScientist{(int)scientist->posx, (int)scientist->posy,
							   scientist->dest.w, scientist->dest.h};

		if (Collision::AABB(tempPlayer, tempScientist)) {
			// Only decrease life if cooldown has passed
			if (collisionCooldown <= 0) {
				life--;
				collisionCooldown = 1000.0f;  // Reset cooldown to 1 second (or
											  // whatever time works best)
			}

			break;
		}
	}
	std::cout << "lifes: " << life << " col cooldown: " << collisionCooldown
			  << std::endl;

	// Update cooldown timer (assuming 'ura->delta' is the time passed per
	// frame)

	if (life <= 0) {
		isRunning = false;
	}
	Camera.x = (int)player->posx - 1920 / 2;
	Camera.y = (int)player->posy - 1080 / 2;
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
