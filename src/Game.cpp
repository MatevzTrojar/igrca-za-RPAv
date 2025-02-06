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
#include "Mouse.hpp"
#include "SDL_cpuinfo.h"
#include "SDL_platform.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/detail/type_vec4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"

std::set<Bullet*>bullets;
GameObject* player,*neki;
std::set<Scientist*> scientists;
Mouse mouse;
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
			SDL_SetRenderDrawColor(renderer, 100, 100,100, 100);
		}

		isRunning = true;
	}
	srand(time(NULL));

	player = new GameObject("assets/textures/Test2.png", 1920 / 2, 1080 / 2);
    neki = new GameObject("assets/textures/Test3.png",300,300);
    for(int i = 0;i<3;i++){
        scientists.insert(new Scientist("assets/textures/scientist.png", rand() % (1920),
                              rand() % (1080)));
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
	//	std::cout << mouse.x << "   " << mouse.y << "  " << mouse.click
	//			  << std::endl;
}
glm::vec2 FinalMove;
int TimeSinceLastBullet = 1e9;
void Game::update(Clock* ura) {
	player->Update(ura);
    std::cout << player->posx << " " << player->posy << std::endl;
    player -> CollisionDetect(neki);
    for(Scientist* scientist:scientists)
	scientist->Update(ura, player);
	if (mouse.click && TimeSinceLastBullet > 750) {
		if (TimeSinceLastBullet > 750) TimeSinceLastBullet = 0;
		Bullet* bullet = new Bullet("assets/textures/bullet.png",
									player->dest.x, player->dest.y);
		bullet->Active = true;
		glm::vec2 vec;
		vec.x = mouse.xpos - player->posx;
		vec.y = mouse.ypos - player->posy;
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
        for(Scientist* scientist:scientists){
            if(bullet->CollisionDetect(scientist)){
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
    for(Scientist* scientist:toDeleteScientist){
        scientists.erase(scientist);
        delete scientist;
    }
    for(Scientist* scientist:scientists){
        if(player->CollisionDetect(scientist)){
            isRunning = false;
        }
    }
}

void Game::render() {
	SDL_RenderClear(renderer);
	player->Render();
    neki->Render();
    for(Scientist* scientist:scientists)
	scientist->Render();
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
