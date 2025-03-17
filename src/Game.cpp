#include "Game.h"

#include <SDL_ttf.h>

#include <algorithm>
#include <cfenv>
#include <climits>
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
#include "TextureManager.h"
#include "glm/detail/type_half.hpp"
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
bool isInnitialized = false;
bool Game::overworld = true;
void Game::RestartGame() {
    victory = false;
    gameOver = false;
    life = 3;
    if (!overworld) {
        for (Bullet* bullet : bullets) delete bullet;
        bullets.clear();
        for (Scientist* scientist : scientists) delete scientist;
        scientists.clear();
        player->posx = 56 * 32;
        player->posy = 25 * 32;
    }
}
Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	int flags = 0;
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError()
				  << std::endl;
		isRunning = false;
		return;
	}
	font = TTF_OpenFont("assets/fonts/Aerial.ttf", 24);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
	}
	TextureManager::RenderText(font, "Lives: " + std::to_string(life),
							   textColor);
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
		}

		isRunning = true;
	}
		Overworldinit();

	srand(time(NULL));
	victoryScreen = TextureManager::LoadTexture("assets/textures/victory.png");
	gameOverScreen =
		TextureManager::LoadTexture("assets/textures/gameover.png");

	isInnitialized = true;
}

bool just_spawned = false;
void Game::Dungeoninit() {
    player->posx = 56 * 32;
    player->posy = 25 * 32;
    map->Bitmap = TextureManager::LoadTexture("assets/textures/Dungeon.png");
    map->checkOverWorld();
    map->AssignRand();
    map->LoadMap();
    map->AssignBorders();

    just_spawned = true;

    std::vector<std::vector<int>> RoomSpawn = {
        {400, 300}, 
        {42 * 32, 30 * 32, 72 * 32, 14 * 32},
        {72 * 32, 43 * 32, 102 * 32, 50 * 32}, 
        {111 * 32, 11 * 32},
        {14 * 32, 56 * 32, 24 * 32, 65 * 32}};
    for (const std::vector<int>& room : RoomSpawn) {
        for (size_t i = 0; i < room.size(); i += 2) {
            scientists.insert(new Scientist("assets/textures/scientist.png", room[i], room[i + 1], 64, 64));
        }
    }
    neki = new GameObject("assets/textures/cage.png", 300, 300, 64, 64);
}
void Game::Overworldinit() {

	player =
		new Player("assets/textures/Chewbacca.png", 30 * 32, 25 * 32, 64, 64);
	map = new Map("assets/textures/overworld.xcf");
}

void Game::handleEvents() {
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_r && (victory || gameOver)) {
				RestartGame();
			}
			break;
		default:
			break;
	}
	mouse.XY(event);
	mouse.Clicked(event);
}
int TimeSinceLastBullet = 1e9;
float collisionCooldown = 0;
bool FollowPlayer = false;
void Game::update(Clock* ura) {
	if (!isInnitialized) {
		return;
	}
    if(just_spawned){
        just_spawned = false;
        return;
    }
	player->Update(ura);
    std::cout<<player->posx<<" "<<player->posy<<std::endl;
	if (overworld) {
		Overworldupdate(ura);
	} else {
		Dungeonupdate(ura);
	}
	if (lifeTextTexture) SDL_DestroyTexture(lifeTextTexture);

	std::string lifeText = "Lives: " + std::to_string(life);
	lifeTextTexture = TextureManager::RenderText(font, lifeText, textColor);

	if (victory || gameOver) {
		return;
	}

	if (scientists.empty() && FollowPlayer) {
		victory = true;
	}

	if (life <= 0) {
		gameOver = true;
	}

	Camera.x = (int)player->posx - 1920 / 2;
	Camera.y = (int)player->posy - 1080 / 2;
int mapWidth = 120 * 32;
int mapHeight = 72 * 32;
if (Camera.x < 0) Camera.x = 0;
if (Camera.y < 0) Camera.y = 0;
if (Camera.x > (mapWidth - Camera.w)) Camera.x = mapWidth - Camera.w;
if (Camera.y > (mapHeight - Camera.h)) Camera.y = mapHeight - Camera.h;
}
void Game::Dungeonupdate(Clock* ura) {
	for (SDL_Rect Border : map->Borders) {
		player->CollisionDetect(Border);
		for (Scientist* scientist : scientists) {
			scientist->CollisionDetect(Border);
		}
		neki->CollisionDetect(Border);
	}

	neki->Update();
	if (Collision::AABB(player->dest, neki->dest)) {
		neki->objTexture =
			TextureManager::LoadTexture("assets/textures/hamster.png");
		FollowPlayer = true;
		neki->dest.w = 64;
		neki->dest.h = 64;
	}
	if (FollowPlayer) {
		neki->FollowPlayer(player, ura);
	}
	for (Scientist* scientist : scientists) scientist->Update(ura, player, map);
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
			if (collisionCooldown <= 0) {
				life--;
				collisionCooldown = 1000.0f;
			}

			break;
		}
	}
}
void Game::Overworldupdate(Clock* ura) {
    for (SDL_Rect Border : map->OWBorders) {
        player->CollisionDetect(Border);
        if (Collision::AABB(player->dest, Border) && map->map[(int)player->posx / 32][(int)player->posy / 32] == '4') {
            Game::overworld = false;
            Dungeoninit();
            return;
        }
    }
}

void Game::render() {
	SDL_RenderClear(renderer);
	SDL_Rect Fullscreen = {0, 0, 1920, 1080};

	if (victory) {
		if (victoryScreen) {
			SDL_RenderCopy(renderer, victoryScreen, nullptr, &Fullscreen);
		} else {
			std::cerr << "Error: victoryScreen texture is null!\n";
		}
	} else if (gameOver) {
		if (gameOverScreen) {
			SDL_RenderCopy(renderer, gameOverScreen, nullptr, &Fullscreen);
		} else {
			std::cerr << "Error: gameOverScreen texture is null!\n";
		}
	} else {
		// SAFETY CHECKS
		if (map) {
			map->Render();
		} else {
			std::cerr << "Error: map is null!\n";
		}

		if (player) {
			player->Render();
		} else {
			std::cerr << "Error: player is null!\n";
		}

		if (!overworld) {
			if (neki) {
				neki->Render();
			} else {
				std::cerr << "Error: neki is null!\n";
			}

			// Check for nullptr in scientists list
			for (Scientist* scientist : scientists) {
				if (scientist) {
					scientist->Render();
				} else {
					std::cerr << "Error: scientist is null!\n";
				}
			}
		}

		// Check for nullptr in bullets list
		for (Bullet* bullet : bullets) {
			if (bullet && bullet->Active) {
				bullet->Render();
			} else if (bullet == nullptr) {
				std::cerr << "Error: bullet is null!\n";
			}
		}

		// Check if lifeTextTexture is valid before rendering
		if (!victory && !gameOver && lifeTextTexture) {
			SDL_Rect lifeRect = {1720, 20, 180, 50};
			SDL_RenderCopy(renderer, lifeTextTexture, nullptr, &lifeRect);
		} else if (!victory && !gameOver) {
			std::cerr << "Warning: lifeTextTexture is null!\n";
		}
	}

	SDL_RenderPresent(renderer);
}


void Game::clean() {
	if (lifeTextTexture) SDL_DestroyTexture(lifeTextTexture);
	TTF_CloseFont(font);
	TTF_Quit();
	if (victoryScreen) SDL_DestroyTexture(victoryScreen);
	if (gameOverScreen) SDL_DestroyTexture(gameOverScreen);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
