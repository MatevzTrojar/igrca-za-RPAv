#include "Game.h"

#include <cfenv>
#include <cstdlib>

#include "Bullet.hpp"
#include "GameObject.h"
#include "Mouse.hpp"
#include "SDL_platform.h"
#include "Scientist.hpp"
#include "TextureManager.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"
GameObject* player;
Scientist* scientist;
Mouse mouse;
Bullet* bullet = NULL;
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
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		}

		isRunning = true;
	}
	srand(time(NULL));

	player = new GameObject("assets/textures/Test2.png", 1920 / 2, 1080 / 2);
	scientist = new Scientist("assets/textures/scientist.png", rand() % (1920),
							  rand() % (1080));
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
	std::cout << mouse.x << "   " << mouse.y << "  " << mouse.click
			  << std::endl;
}
    glm::vec2 FinalMove;

void Game::update(Clock* ura) {
	player->Update(ura);
	scientist->Update(ura, player);
	if (mouse.click) {
		glm::vec2 vec;
		vec.x = mouse.xpos - player->posx;
		vec.y = mouse.ypos - player->posy;
		bullet = new Bullet("assets/textures/bullet.png", player->dest.x,
							player->dest.x);
        FinalMove = glm::normalize(vec);
		bullet->Active = true;
	}
	if (bullet != NULL) {
        bullet->Update(FinalMove,ura);
	}

}

void Game::render() {
	SDL_RenderClear(renderer);
	player->Render();
	scientist->Render();
    if(bullet!=NULL){
        bullet->Render();
    }
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
