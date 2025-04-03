#include "Game.h"

#include <SDL_ttf.h>

#include <climits>
#include <cstddef>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <set>
#include <vector>

#include "Bullet.hpp"
#include "Collision.hpp"
#include "GameObject.h"
#include "Mouse.hpp"
#include "SDL_blendmode.h"
#include "SDL_events.h"
#include "SDL_log.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "Scientist.hpp"
#include "TextureManager.h"
#include "glm/detail/type_half.hpp"
#include "player.hpp"
int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;
std::set<Bullet*> bullets;
Player* player;
GameObject* Level1Pet;
GameObject* Level2Pet;
GameObject* Level3Pet;
std::set<Scientist*> scientists;
Mouse mouse;
Map* map;
SDL_Rect Game::Camera = {0, 0, 1920, 1080};
int life = 3;
SDL_Renderer* Game::renderer = nullptr;
SDL_Renderer* PauseRenderer = nullptr;
bool isInnitialized = false;
bool Game::overworld = true;
bool Game::victory = false;
bool Game::gameOver = false;
bool Game::Paused = false;
int Game::level = 0;
TTF_Font* pauseFont = nullptr;
void Game::RestartGame() {
	if (Level1Pet) {
		delete Level1Pet;
		Level1Pet = nullptr;
	}
	if (Level2Pet) {
		delete Level2Pet;
		Level2Pet = nullptr;
	}
	if (Level3Pet) {
		delete Level3Pet;
		Level3Pet = nullptr;
	}
	victory = false;
	gameOver = false;
	Paused = false;
	life = 3;
	if (!overworld) {
		overworld = true;
		Overworldinit();
	}
	level = 0;
	for (int x = 0; x < 120; x++) {
		for (int y = 0; y < 72; y++) {
			map->tile[x][y].Used = false;
		}
	}
	scientists.clear();
}
void Game::ContinueGame() {
	victory = false;
	gameOver = false;
	overworld = true;
	Overworldinit();
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
    pauseFont = TTF_OpenFont("assets/fonts/Aerial.ttf", 72);
    if (!pauseFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
	}
	TextureManager::RenderText(font, "Hitpoints: " + std::to_string(life),
							   textColor);
    TextureManager::RenderText(pauseFont, "Paused", textColor);
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		isRunning = true;
	}
	Overworldinit();

	srand(time(NULL));
	victoryScreen = TextureManager::LoadTexture("assets/textures/victory.xcf");
	gameOverScreen =
		TextureManager::LoadTexture("assets/textures/gameover.png");
	pauseScreen =
		TextureManager::LoadTexture("assets/textures/blackscreen.jpg");

	isInnitialized = true;
}

bool just_spawned = false;

void Game::Dungeoninit() {
	map->Bitmap = TextureManager::LoadTexture("assets/textures/Dungeon.png");
	map->checkOverWorld();
	map->AssignRand();
	map->LoadMap();
	map->AssignBorders();
	just_spawned = true;
	std::vector<std::vector<int>> RoomSpawn;
	if (level == 1) {
		player->posx = 56 * 32;
		player->posy = 25 * 32;

		RoomSpawn = {{400, 300},
					 {42 * 32, 30 * 32, 72 * 32, 14 * 32},
					 {72 * 32, 43 * 32, 102 * 32, 50 * 32},
					 {111 * 32, 11 * 32},
					 {14 * 32, 56 * 32, 24 * 32, 65 * 32}};

		Level1Pet =
			new GameObject("assets/textures/cage.png", 300, 300, 64, 64);
	} else if (level == 2) {
		Level1Pet->posx = -100;
		Level1Pet->posy = -100;
		player->posx = 26 * 32;
		player->posy = 22 * 32;
		RoomSpawn = {{12 * 32, 28 * 32, 41 * 32, 28 * 32},
					 {49 * 32, 4 * 32, 72 * 32, 5 * 32},
					 {98 * 32, 13 * 32, 110 * 32, 25 * 32},
					 {61 * 32, 39 * 32},
					 {51 * 32, 53 * 32, 59 * 32, 62 * 32, 98 * 32, 63 * 32,
					  92 * 32, 55 * 32}};
		Level2Pet = new GameObject("assets/textures/cage.png", 71 * 32, 58 * 32,
								   64, 64);
	}
	if (level == 3) {
		Level1Pet->posx = -100;
		Level1Pet->posy = -100;

		Level2Pet->posx = -100;
		Level2Pet->posy = -100;

		player->posx = 16 * 32;
		player->posy = 4 * 32;
		RoomSpawn = {{33 * 32, 5 * 32},
					 {97 * 32, 5 * 32, 105 * 32, 10 * 32},
					 {60 * 32, 23 * 32, 70 * 32, 27 * 32},
					 {14 * 32, 24 * 32, 27 * 32, 28 * 32},
					 {111 * 32, 25 * 32},
					 {20 * 32, 48 * 32, 33 * 32, 53 * 32, 34 * 32, 48 * 32},
					 {66 * 32, 67 * 32, 79 * 32, 64 * 32},
					 {101 * 32, 50 * 32, 107 * 32, 52 * 32}};
		Level3Pet = new GameObject("assets/textures/cage.png", 71 * 32, 65 * 32,
								   64, 64);
	}
	for (const std::vector<int>& room : RoomSpawn) {
		for (size_t i = 0; i < room.size(); i += 2) {
			scientists.insert(new Scientist("assets/textures/scientist.png",
											room[i], room[i + 1], 64, 64));
		}
	}
}
void Game::Overworldinit() {
	if (level == 0) {
		player = new Player("assets/textures/Chewbacca.png", 30 * 32, 25 * 32,
							64, 64);
		map = new Map("assets/textures/overworld.xcf");

	} else {
		player->posx = 30 * 32;
		player->posy = 25 * 32;
		map->Bitmap =
			TextureManager::LoadTexture("assets/textures/overworld.xcf");
		map->checkOverWorld();
		map->AssignRand();
		map->LoadMap();
		map->AssignBorders();

		if (Level1Pet && level == 1) {
			Level1Pet->posx = 33 * 32;
			Level1Pet->posy = 19 * 32;
		} else if (Level2Pet && level == 2) {
			Level2Pet->posx = 33 * 32;
			Level2Pet->posy = 19 * 32;
		} else if (Level3Pet && level == 3) {
			Level3Pet->posx = 33 * 32;
			Level3Pet->posy = 19 * 32;
		}
	}
}

void Game::handleEvents() {
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_r) {
				RestartGame();
			}
			if (event.key.keysym.sym == SDLK_p) {
				if (Paused) {
					Paused = false;
				} else {
					Paused = true;
				}
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
void Game::update() {
	if (Paused) {
		return;
	}
	std::cout << Paused << std::endl;
	if (!isInnitialized) {
		return;
	}
	if (just_spawned) {
		just_spawned = false;
		return;
	}
	player->Update();
	if (overworld) {
		Overworldupdate();
	} else {
		Dungeonupdate();
	}
	if (lifeTextTexture) SDL_DestroyTexture(lifeTextTexture);

	std::string lifeText = "Hitpoints: " + std::to_string(life);
	lifeTextTexture = TextureManager::RenderText(font, lifeText, textColor);

	if (victory || gameOver) {
		return;
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
void Game::Dungeonupdate() {
	for (SDL_Rect Border : map->Borders) {
		player->CollisionDetect(Border);
		for (Scientist* scientist : scientists) {
			scientist->CollisionDetect(Border);
		}
		if (level == 1) {
			Level1Pet->CollisionDetect(Border);
		} else if (level == 2) {
			Level2Pet->CollisionDetect(Border);
		} else if (level == 3) {
			Level3Pet->CollisionDetect(Border);
		}
	}
	if (level == 1) {
		Level1Pet->Update();
	} else if (level == 2) {
		Level2Pet->Update();
	} else if (level == 3) {
		Level3Pet->Update();
	}
	if (level == 1) {
		if (Collision::AABB(player->dest, Level1Pet->dest)) {
			Level1Pet->Follow = true;
			Level1Pet->dest.w = 64;
			Level1Pet->dest.h = 64;
		}
		if (Level1Pet->Follow) {
			Level1Pet->FollowPlayer(player);
		}
	} else if (level == 2) {
		if (Collision::AABB(player->dest, Level2Pet->dest)) {
			Level2Pet->Follow = true;
			Level2Pet->dest.w = 64;
			Level2Pet->dest.h = 64;
		}
		if (Level2Pet->Follow) {
			Level2Pet->FollowPlayer(player);
		}
	} else if (level == 3) {
		if (Collision::AABB(player->dest, Level3Pet->dest)) {
			Level3Pet->Follow = true;
			Level3Pet->dest.w = 64;
			Level3Pet->dest.h = 64;
		}
		if (Level3Pet->Follow) {
			Level3Pet->FollowPlayer(player);
		}
	}

	for (Scientist* scientist : scientists) scientist->Update(player, map);
	if (mouse.click && TimeSinceLastBullet > 750) {
		TimeSinceLastBullet = 0;
		Bullet* bullet =
			new Bullet("assets/textures/bullet.png", player->posx + 10,
					   player->posy + 10, 48, 48);
		bullet->Active = true;
		bullet->pos.x = mouse.xpos + Camera.x - player->posx;
		bullet->pos.y = mouse.ypos + Camera.y - player->posy;
		bullet->pos = glm::normalize(bullet->pos);
		bullets.insert(bullet);
	}
	TimeSinceLastBullet += Clock::delta;

	for (Bullet* bullet : bullets) {
		if (bullet != NULL && bullet->Active) {
			bullet->Update(player);
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

	collisionCooldown -= Clock::delta;
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
	if (scientists.empty()) {
		victory = true;
	}
}

void Game::Overworldupdate() {
	if (level == 2) {
		Level1Pet->posx = Level1Pet->ShelterX;
		Level1Pet->posy = Level1Pet->ShelterY;
	}
	if (level == 3) {
		Level1Pet->posx = Level1Pet->ShelterX;
		Level1Pet->posy = Level1Pet->ShelterY;
		Level2Pet->posx = Level2Pet->ShelterX;
		Level2Pet->posy = Level2Pet->ShelterY;
	}
	map->Borders.clear();
	if (Level1Pet) {
		Level1Pet->Update();
		Level1Pet->ShelterDetect();
		Level1Pet->FollowPlayer(player);
	}
	if (Level2Pet) {
		Level2Pet->Update();
		Level2Pet->ShelterDetect();
		Level2Pet->FollowPlayer(player);
	}
	if (Level3Pet) {
		Level3Pet->Update();
		Level3Pet->ShelterDetect();
		Level3Pet->FollowPlayer(player);
	}

	for (SDL_Rect Border : map->OWBorders) {
		player->CollisionDetect(Border);
		if (Level1Pet) Level1Pet->CollisionDetect(Border);
		if (Level2Pet) Level2Pet->CollisionDetect(Border);
		if (Level3Pet) Level3Pet->CollisionDetect(Border);
	}
	if (Level1Pet) player->ShelterDetect();
	SDL_Rect tempPlayer{(int)player->posx, (int)player->posy, player->dest.w,
						player->dest.h};
	Tile* playerTile =
		&map->tile[(int)player->posx / 32][(int)player->posy / 32];
	if (Collision::AABB(tempPlayer, playerTile->dest) &&
		((map->map[(int)player->posx / 32][(int)player->posy / 32] == '4' ||
		  map->map[(int)player->posx / 32][(int)player->posy / 32] == '7') &&
		 !playerTile->Used)) {
		if (Level1Pet && Level1Pet->inShelter && level == 1) {
			Level1Pet->ShelterX = Level1Pet->posx;
			Level1Pet->ShelterY = Level1Pet->posy;
			Game::overworld = false;
			playerTile->Used = true;
			level++;
			Dungeoninit();
			return;
		} else if (Level2Pet && Level2Pet->inShelter && Level1Pet &&
				   Level2Pet->inShelter && level == 2) {
			Level1Pet->ShelterX = Level1Pet->posx;
			Level1Pet->ShelterY = Level1Pet->posy;
			Level2Pet->ShelterX = Level2Pet->posx;
			Level2Pet->ShelterY = Level2Pet->posy;

			Game::overworld = false;
			playerTile->Used = true;
			level++;

			Dungeoninit();
			return;
		} else if (Level3Pet && Level3Pet->inShelter && Level2Pet &&
				   Level3Pet->inShelter && Level1Pet && Level1Pet->inShelter &&
				   level == 3) {
			Game::overworld = false;
			playerTile->Used = true;
			level++;
			Dungeoninit();
			return;
		}
		if (level == 0) {
			Game::overworld = false;
			playerTile->Used = true;
			level++;
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
			SDL_Rect ContinueButton = {488, 381, 1000, 117},
					 mouseRect = {(int)mouse.xpos, (int)mouse.ypos, 1, 1};
			if (Collision::AABB(ContinueButton, mouseRect) && mouse.click &&
				victory) {
				ContinueGame();
			}
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
	}

	else {
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
		if (Level1Pet) Level1Pet->Render();
		if (Level2Pet) Level2Pet->Render();
		if (Level3Pet) Level3Pet->Render();

		// Check for nullptr in scientists list
		for (Scientist* scientist : scientists) {
			if (scientist) {
				scientist->Render();
			} else {
				std::cerr << "Error: scientist is null!\n";
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
	if (Paused) {
		SDL_SetRenderDrawBlendMode(renderer,
								   SDL_BLENDMODE_BLEND);  // Enable transparency
		SDL_SetRenderDrawColor(renderer, 0, 0, 0,
							   128);				// Semi-transparent black
		SDL_RenderFillRect(renderer, &Fullscreen);	// Draw the overlay
        SDL_Texture * pauseTexture =
            TextureManager::RenderText(pauseFont, "Paused", textColor);
        SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 50,
                              400, 100};
        SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);
        SDL_DestroyTexture(pauseTexture);

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
