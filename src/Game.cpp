#include "Game.h"

#include <SDL_blendmode.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>

#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <glm/glm.hpp>
#include <ios>
#include <iostream>
#include <set>
#include <vector>

#include "Bullet.hpp"
#include "Collision.hpp"
#include "GameObject.h"
#include "Mouse.hpp"
#include "Replay.hpp"
#include "Scientist.hpp"
#include "TextureManager.h"
#include "clock.hpp"

int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;
std::set<Bullet*> bullets;
GameObject* Level1Pet;
GameObject* Level2Pet;
GameObject* Level3Pet;
std::set<Scientist*> scientists;
Mouse mouse;
int life = 3;
SDL_Renderer* PauseRenderer = nullptr;
bool isInnitialized = false;
SDL_Renderer* Game::renderer = nullptr;

Player* Game::player;
Map* Game::map;
SDL_Rect Game::Camera = {0, 0, 1920, 1080};
bool Game::overworld = true;
bool Game::victory = false;
bool Game::gameOver = false;
bool Game::MainMenu = true;
bool Game::EndGame = false;
bool Game::Paused = false;
std::vector<Game::DeltaPlayer> Game::playerReplayPath;
bool Game::recordingReplay = false;
bool Game::playingReplay = false;
int Game::replayFrame = 0;
SDL_Point Game::LeftDungeon1 = {0, 0};
SDL_Point Game::LeftDungeon2 = {0, 0};
SDL_Point Game::LeftDungeon3 = {0, 0};
Player* ghostPlayer = nullptr;

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
	EndGame = false;
	Overworldinit();
}
void Game::SaveReplay() {
	std::ofstream ReplayFile("replay.bin", std::ios::binary);
	if (!ReplayFile) {
		std::cerr << "Error opening file for saving." << std::endl;
		return;
	}
	Game::ReplayData replayData;
	replayData.LeftDungeon1 = Game::LeftDungeon1;
	replayData.LeftDungeon2 = Game::LeftDungeon2;
	replayData.LeftDungeon3 = Game::LeftDungeon3;

	ReplayFile.write(reinterpret_cast<const char*>(&replayData),
					 sizeof(Game::ReplayData));
	for (const DeltaPlayer& point : playerReplayPath) {
		ReplayFile.write(reinterpret_cast<const char*>(&point), sizeof(point));
	}

	std::cout << "ReplayData.LeftDungeon1.x/y: " << Game::LeftDungeon1.x
			  << Game::LeftDungeon1.y << std::endl;

	ReplayFile.close();
	std::cout << "Replay saved successfully!" << std::endl;
}
struct SaveData {
	int level;
	int life;
	bool overworld;
	bool victory;
	bool gameOver;
	bool EndGame;
	int PetCount;
	bool Follow1, Follow2, Follow3;
};

void Game::SaveGame() {
	std::ofstream SaveFile("Saving.bin", std::ios::binary);
	if (!SaveFile) {
		std::cerr << "Error opening file for saving." << std::endl;
		return;
	}
	SaveData data;
	data.level = level;
	data.life = life;
	data.overworld = overworld;
	data.victory = victory;
	data.gameOver = gameOver;
	data.EndGame = EndGame;
	data.Follow1 = Level1Pet ? Level1Pet->Follow : false;
	data.Follow2 = Level2Pet ? Level2Pet->Follow : false;
	data.Follow3 = Level3Pet ? Level3Pet->Follow : false;

	data.PetCount = (Level1Pet != nullptr) + (Level2Pet != nullptr) +
					(Level3Pet != nullptr);

	SaveFile.write(reinterpret_cast<const char*>(&data), sizeof(SaveData));

	SaveObjectData saveData;
	if (Level1Pet) {
		saveData = Level1Pet->GetSaveData();
		SaveFile.write(reinterpret_cast<char*>(&saveData), sizeof(saveData));
	}
	if (Level2Pet) {
		saveData = Level2Pet->GetSaveData();
		SaveFile.write(reinterpret_cast<char*>(&saveData), sizeof(saveData));
	}
	if (Level3Pet) {
		saveData = Level3Pet->GetSaveData();
		SaveFile.write(reinterpret_cast<char*>(&saveData), sizeof(saveData));
	}
	if (overworld) {
		SaveObjectData playerData = player->GetSaveData();
		SaveFile.write(reinterpret_cast<char*>(&playerData),
					   sizeof(SaveObjectData));
	}

	SaveFile.close();
	std::cout << "Level: " << level << std::endl;
	std::cout << "Life: " << life << std::endl;
	std::cout << "Overworld: " << overworld << std::endl;
	std::cout << "Victory: " << victory << std::endl;
	std::cout << "GameOver: " << gameOver << std::endl;
	std::cout << "EndGame: " << EndGame << std::endl;
	if (Level1Pet) {
		std::cout << "Level1Pet: " << Level1Pet->posx << ", " << Level1Pet->posy
				  << std::endl;
	}
	if (Level2Pet) {
		std::cout << "Level2Pet: " << Level2Pet->posx << ", " << Level2Pet->posy
				  << std::endl;
	}
	if (Level3Pet) {
		std::cout << "Level3Pet: " << Level3Pet->posx << ", " << Level3Pet->posy
				  << std::endl;
	}
	std::cout << "Game saved successfully!" << std::endl;
}

bool Game::Loaded = false;
void Game::LoadGame() {
	Loaded = true;
	std::ifstream ReplayFile("replay.bin", std::ios::binary);
	Game::ReplayData replayData;
	ReplayFile.read(reinterpret_cast<char*>(&replayData),
					sizeof(Game::ReplayData));
	if (!ReplayFile) {
		std::cerr << "Error opening file for loading." << std::endl;
		return;
	}


	Game::LeftDungeon1 = replayData.LeftDungeon1;
	Game::LeftDungeon2 = replayData.LeftDungeon2;
	Game::LeftDungeon3 = replayData.LeftDungeon3;
	DeltaPlayer point;
	while (ReplayFile.read(reinterpret_cast<char*>(&point), sizeof(point))) {
		playerReplayPath.push_back(point);
	}
	ReplayFile.close();
	std::ifstream LoadFile("Saving.bin", std::ios::binary);
	if (!LoadFile) {
		std::cerr << "Error opening file for loading." << std::endl;
		return;
	}

	SaveData data;
	LoadFile.read(reinterpret_cast<char*>(&data), sizeof(SaveData));
	level = data.level;
	life = data.life;
	overworld = data.overworld;
	victory = data.victory;
	gameOver = data.gameOver;
	EndGame = data.EndGame;

	auto loadPet = [&](GameObject*& pet, bool followState,
					   const char* texture) {
		SaveObjectData loadData;
		if (!LoadFile.read(reinterpret_cast<char*>(&loadData),
						   sizeof(loadData))) {
			std::cerr << "Failed to read pet data!" << std::endl;
			return;
		}
		pet = new GameObject(texture, loadData.posx, loadData.posy, 64, 64);
		pet->SetSaveData(loadData);
		pet->Follow = followState;
		pet->ResetAnimation();
	};

	if (data.PetCount >= 1) {
		loadPet(Level1Pet, data.Follow1, "assets/textures/hampter.png");
		std::cout << "Level1Pet shelter x/y: " << Level1Pet->ShelterX << ", "
				  << Level1Pet->ShelterY << std::endl;
	}
	if (data.PetCount >= 2)
		loadPet(Level2Pet, data.Follow2, "assets/textures/hampter.png");
	if (data.PetCount >= 3)
		loadPet(Level3Pet, data.Follow3, "assets/textures/hampter.png");
	if (overworld) {
		SaveObjectData playerData;
		if (!LoadFile.read(reinterpret_cast<char*>(&playerData),
						   sizeof(playerData))) {
			std::cerr << "Failed to read player data!" << std::endl;
			return;
		}
		if (!player) {
			player = new Player("assets/textures/Chewbacca.png", 30 * 32,
								25 * 32, 64, 64);
		}

		player->SetSaveData(playerData);
	} else {
		if (level == 1) {
			if (Level1Pet) {
				Level1Pet->posx = -100;
				Level1Pet->posy = -100;
			} else {
				Level1Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}
		}
		if (level == 2) {
			if (Level1Pet) {
				Level1Pet->posx = -100;
				Level1Pet->posy = -100;
			} else {
				Level1Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}
			if (Level2Pet) {
				Level2Pet->posx = -100;
				Level2Pet->posy = -100;
			} else {
				Level2Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}
		}
		if (level == 3) {
			if (Level1Pet) {
				Level1Pet->posx = -100;
				Level1Pet->posy = -100;
			} else {
				Level1Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}
			if (Level2Pet) {
				Level2Pet->posx = -100;
				Level2Pet->posy = -100;
			} else {
				Level2Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}
			Level3Pet = new GameObject("assets/textures/hampter.png", 71 * 32,
									   65 * 32, 64, 64);
		}
	}
	LoadFile.close();
	std::cout << "Level: " << level << std::endl;
	std::cout << "Life: " << life << std::endl;
	std::cout << "Overworld: " << overworld << std::endl;
	std::cout << "Victory: " << victory << std::endl;
	std::cout << "GameOver: " << gameOver << std::endl;
	std::cout << "EndGame: " << EndGame << std::endl;
	if (Level1Pet) {
		std::cout << "Level1Pet: " << Level1Pet->posx << ", " << Level1Pet->posy
				  << std::endl;
	}
	if (Level2Pet) {
		std::cout << "Level2Pet: " << Level2Pet->posx << ", " << Level2Pet->posy
				  << std::endl;
	}
	if (Level3Pet) {
		std::cout << "Level3Pet: " << Level3Pet->posx << ", " << Level3Pet->posy
				  << std::endl;
	}
	std::cout << "Game loaded successfully!" << std::endl;
	if (overworld) {
		Overworldinit();
	} else {
		if (Level1Pet)
			std::cout << "Level1Pet shelter x/y: " << Level1Pet->ShelterX
					  << ", " << Level1Pet->ShelterY << std::endl;
		Dungeoninit();
	}
	Loaded = false;
}

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	int flags = SDL_WINDOW_RESIZABLE;
	if (fullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}

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

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		isRunning = true;
	} else {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
				  << std::endl;
	}

	renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
									 SDL_TEXTUREACCESS_TARGET, 1920, 1080);

	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	Overworldinit();

	srand(time(NULL));
	victoryScreen = TextureManager::LoadTexture("assets/textures/victory.xcf");
	EndGameScreen = TextureManager::LoadTexture("assets/textures/EndGame.png");
	gameOverScreen =
		TextureManager::LoadTexture("assets/textures/gameover.png");
	mainMenuTexture =
		TextureManager::LoadTexture("assets/textures/MainMenu.png");
	EndGameScreen = TextureManager::LoadTexture("assets/textures/EndGame.PNG");
	isInnitialized = true;
}
bool just_spawned = false;

void Game::Dungeoninit(bool isReplay) {
	switch (level) {
		case 1:
			player->posx = 56 * 32;
			player->posy = 25 * 32;
			break;
		case 2:
			player->posx = 26 * 32;
			player->posy = 22 * 32;
			break;
		case 3:
			player->posx = 16 * 32;
			player->posy = 4 * 32;
			break;
	}
	map->Bitmap = TextureManager::LoadTexture("assets/textures/Dungeon.png");
	map->checkOverWorld();
	map->AssignRand();
	map->LoadMap();
	map->AssignBorders();
	just_spawned = true;

	if (isReplay) {
		return;
	} else {
		std::vector<std::vector<int>> RoomSpawn;
		if (level == 1) {
			RoomSpawn = {{400, 300},
						 {42 * 32, 30 * 32, 72 * 32, 14 * 32},
						 {72 * 32, 43 * 32, 102 * 32, 50 * 32},
						 {111 * 32, 11 * 32},
						 {14 * 32, 56 * 32, 24 * 32, 65 * 32}};

			Level1Pet =
				new GameObject("assets/textures/cage.png", 300, 300, 64, 64);
		} else if (level == 2) {
			if (Level1Pet) {
				Level1Pet->posx = -100;
				Level1Pet->posy = -100;
				std::cout << "Dungeon init Level1Pet shelter x/y: "
						  << Level1Pet->ShelterX << ", " << Level1Pet->ShelterY
						  << std::endl;
			} else {
				Level1Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}

			RoomSpawn = {{12 * 32, 28 * 32, 41 * 32, 28 * 32},
						 {49 * 32, 4 * 32, 72 * 32, 5 * 32},
						 {98 * 32, 13 * 32, 110 * 32, 25 * 32},
						 {61 * 32, 39 * 32},
						 {51 * 32, 53 * 32, 59 * 32, 62 * 32, 98 * 32, 63 * 32,
						  92 * 32, 55 * 32}};
			Level2Pet = new GameObject("assets/textures/cage.png", 71 * 32,
									   58 * 32, 64, 64);
			Level2Pet->ResetAnimation();
		}
		if (level == 3) {
			if (Level1Pet) {
				Level1Pet->posx = -100;
				Level1Pet->posy = -100;
			} else {
				Level1Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}
			if (Level2Pet) {
				Level2Pet->posx = -100;
				Level2Pet->posy = -100;
			} else {
				Level2Pet = new GameObject("assets/textures/hampter.png", -100,
										   -100, 64, 64);
			}
			RoomSpawn = {{33 * 32, 5 * 32},
						 {97 * 32, 5 * 32, 105 * 32, 10 * 32},
						 {60 * 32, 23 * 32, 70 * 32, 27 * 32},
						 {14 * 32, 24 * 32, 27 * 32, 28 * 32},
						 {111 * 32, 25 * 32},
						 {20 * 32, 48 * 32, 33 * 32, 53 * 32, 34 * 32, 48 * 32},
						 {66 * 32, 67 * 32, 79 * 32, 64 * 32},
						 {101 * 32, 50 * 32, 107 * 32, 52 * 32}};
			if (!Loaded && !Level3Pet)
				Level3Pet = new GameObject("assets/textures/cage.png", 71 * 32,
										   65 * 32, 64, 64);
			Level3Pet->objTexture =
				TextureManager::LoadTexture("assets/textures/cage.png");
			Level3Pet->ResetAnimation();
		}
		for (const std::vector<int>& room : RoomSpawn) {
			for (size_t i = 0; i < room.size(); i += 2) {
				scientists.insert(new Scientist("assets/textures/scientist.png",
												room[i], room[i + 1], 64, 64));
			}
		}
	}
}

void Game::Overworldinit(bool isReplay) {
	if (isReplay) {
		if (!map) {
			map = new Map("assets/textures/overworld.xcf");
		}
		if (!player) {
			player = new Player("assets/textures/Chewbacca.png", 30 * 32,
								25 * 32, 64, 64);
		}
		player->posx = 30 * 32;
		player->posy = 25 * 32;
		map->Bitmap =
			TextureManager::LoadTexture("assets/textures/overworld.xcf");
		map->checkOverWorld();
		map->AssignRand();
		map->LoadMap();
		map->AssignBorders();
		return;
	}
	if (level == 0) {
		player = new Player("assets/textures/Chewbacca.png", 30 * 32, 25 * 32,
							64, 64);
		map = new Map("assets/textures/overworld.xcf");
	} else {
		map->Bitmap =
			TextureManager::LoadTexture("assets/textures/overworld.xcf");
		map->checkOverWorld();
		map->AssignRand();
		map->LoadMap();
		map->AssignBorders();

		if (Level1Pet) {
			Level1Pet->FollowPlayer(player);
		}

		if (Level2Pet) {
			Level2Pet->FollowPlayer(player);
		}
		if (Level3Pet) {
			Level3Pet->FollowPlayer(player);
		}
		if (!Loaded) {
			player->posx = 30 * 32;
			player->posy = 25 * 32;

			if (Level1Pet && level == 1) {
				Level1Pet->posx = 33 * 32;
				Level1Pet->posy = 19 * 32;
				Level1Pet->ResetAnimation();
			}
			if (Level2Pet && level == 2) {
				Level2Pet->posx = 33 * 32;
				Level2Pet->posy = 19 * 32;
				Level2Pet->ResetAnimation();
			}
			if (Level3Pet && level == 3) {
				Level3Pet->posx = 33 * 32;
				Level3Pet->posy = 19 * 32;
				Level3Pet->ResetAnimation();
			}
		}
	}
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				running(true);
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					SCREEN_WIDTH = event.window.data1;
					SCREEN_HEIGHT = event.window.data2;
				}
				break;
			case SDL_MOUSEBUTTONDOWN: {
				int windowW, windowH;
				SDL_GetWindowSize(window, &windowW, &windowH);

				float scaleX = 1920.0f / windowW;
				float scaleY = 1080.0f / windowH;
				mouse.xpos = event.button.x * scaleX;
				mouse.ypos = event.button.y * scaleY;

				SDL_Rect mouseRect = {static_cast<int>(mouse.xpos),
									  static_cast<int>(mouse.ypos), 1, 1};

				if (MainMenu) {
					if (Collision::AABB(playButton, mouseRect)) {
						MainMenu = false;
						RestartGame();
					} else if (Collision::AABB(loadButton, mouseRect)) {
						LoadGame();
						MainMenu = false;
					} else if (Collision::AABB(settingsButton, mouseRect)) {
						MainMenu = false;
						playingReplay = true;
						Replay::initReplay(this);
					} else if (Collision::AABB(UserButton, mouseRect)) {
					}
				}
				if (victory) {
					if (Collision::AABB(continueButton, mouseRect)) {
						victory = false;
						ContinueGame();
					}
				}
				if (EndGame) {
					if (Collision::AABB(BackToMainMenu, mouseRect)) {
						EndGame = false;
						MainMenu = true;
						RestartGame();
					}
				}
			} break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_r) {
					RestartGame();
				}
				if (event.key.keysym.sym == SDLK_p) {
					Paused = !Paused;
				}
				break;
			default:
				break;
		}
		mouse.XY(event);
		mouse.Clicked(event);
	}
}

int TimeSinceLastBullet = 1e9;
float collisionCooldown = 0;
static Uint32 lastCaptureTime = 0;
void Game::update() {
	if (Paused) {
		return;
	}
	if (playingReplay) {
		Replay::Update(this);
		Camera.x = static_cast<int>(player->posx) - 1920 / 2;
		Camera.y = static_cast<int>(player->posy) - 1080 / 2;
		int mapWidth = 120 * 32;
		int mapHeight = 72 * 32;
		if (Camera.x < 0) Camera.x = 0;
		if (Camera.y < 0) Camera.y = 0;
		if (Camera.x > (mapWidth - Camera.w)) Camera.x = mapWidth - Camera.w;
		if (Camera.y > (mapHeight - Camera.h)) Camera.y = mapHeight - Camera.h;
		return;
	}

	if (!isInnitialized) {
		return;
	}
	if (just_spawned) {
		just_spawned = false;
		return;
	}

	Uint32 now = SDL_GetTicks();
	if (now - lastCaptureTime >= 1000 / 60) {
		lastCaptureTime = now;

		DeltaPlayer deltaPlayer = {player->posx, player->posy};
		playerReplayPath.push_back(deltaPlayer);
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

	if (victory || gameOver || EndGame) {
		return;
	}

	if (life <= 0) {
		gameOver = true;
	}

	Camera.x = static_cast<int>(player->posx) - 1920 / 2;
	Camera.y = static_cast<int>(player->posy) - 1080 / 2;
	int mapWidth = 120 * 32;
	int mapHeight = 72 * 32;
	if (Camera.x < 0) Camera.x = 0;
	if (Camera.y < 0) Camera.y = 0;
	if (Camera.x > (mapWidth - Camera.w)) Camera.x = mapWidth - Camera.w;
	if (Camera.y > (mapHeight - Camera.h)) Camera.y = mapHeight - Camera.h;
}

void Game::Dungeonupdate(bool isReplay) {
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
		if (bullet != nullptr && bullet->Active) {
			bullet->Update(player);
		}
	}

	for (Bullet* bullet : bullets) {
		for (SDL_Rect Border : map->Borders) {
			SDL_Rect tempBullet{static_cast<int>(bullet->posx),
								static_cast<int>(bullet->posy), bullet->dest.w,
								bullet->dest.h};
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
		SDL_Rect tempPlayer{static_cast<int>(player->posx),
							static_cast<int>(player->posy), player->dest.w,
							player->dest.h};
		SDL_Rect tempScientist{static_cast<int>(scientist->posx),
							   static_cast<int>(scientist->posy),
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
		switch (level) {
			case 1:
				LeftDungeon1.x = player->posx;
				LeftDungeon1.y = player->posy;
				std::cout << "LeftDungeon1.x/y: " << LeftDungeon1.x
						  << LeftDungeon1.y << std::endl;
				break;
			case 2:
				LeftDungeon2.x = player->posx;
				LeftDungeon2.y = player->posy;
				break;
			case 3:
				LeftDungeon3.x = player->posx;
				LeftDungeon3.y = player->posy;
				break;
		}
		victory = true;
	}
}

void Game::Overworldupdate(bool isReplay) {
	if (Level1Pet && Level1Pet->inShelter && Level2Pet &&
		Level2Pet->inShelter && Level3Pet && Level3Pet->inShelter) {
		EndGame = true;
	}
	if (level == 2) {
		if (Level1Pet) {
			Level1Pet->posx = Level1Pet->ShelterX;
			Level1Pet->posy = Level1Pet->ShelterY;
		}
	}
	if (level == 3) {
		if (Level1Pet) {
			Level1Pet->posx = Level1Pet->ShelterX;
			Level1Pet->posy = Level1Pet->ShelterY;
		}
		if (Level2Pet) {
			Level2Pet->posx = Level2Pet->ShelterX;
			Level2Pet->posy = Level2Pet->ShelterY;
		}
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
	SDL_Rect tempPlayer{static_cast<int>(player->posx),
						static_cast<int>(player->posy), player->dest.w,
						player->dest.h};
	Tile* playerTile = &map->tile[static_cast<int>(player->posx) / 32]
								 [static_cast<int>(player->posy) / 32];
	if (Collision::AABB(tempPlayer, playerTile->dest) &&
		((map->map[static_cast<int>(player->posx) / 32]
				  [static_cast<int>(player->posy) / 32] == '4' ||
		  map->map[static_cast<int>(player->posx) / 32]
				  [static_cast<int>(player->posy) / 32] == '7') &&
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
	SDL_SetRenderTarget(renderer, renderTarget);
	SDL_RenderClear(renderer);

	SDL_Rect logicalRect = {0, 0, 1920, 1080};
	if (MainMenu) {
		SDL_RenderCopy(renderer, mainMenuTexture, nullptr, &logicalRect);
	} else if (EndGame) {
		SDL_RenderCopy(renderer, EndGameScreen, nullptr, &logicalRect);
	} else if (victory) {
		SDL_RenderCopy(renderer, victoryScreen, nullptr, &logicalRect);
	} else if (gameOver) {
		SDL_RenderCopy(renderer, gameOverScreen, nullptr, &logicalRect);
	} else {
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
		if (!playingReplay) {
			if (Level1Pet) Level1Pet->Render();
			if (Level2Pet) Level2Pet->Render();
			if (Level3Pet) Level3Pet->Render();

			for (Scientist* scientist : scientists) {
				if (scientist)
					scientist->Render();
				else
					std::cerr << "Error: scientist is null!\n";
			}

			for (Bullet* bullet : bullets) {
				if (bullet && bullet->Active)
					bullet->Render();
				else if (bullet == nullptr)
					std::cerr << "Error: bullet is null!\n";
			}

			if (!victory && !gameOver && lifeTextTexture) {
				SDL_Rect lifeRect = {1720, 20, 180, 50};
				SDL_RenderCopy(renderer, lifeTextTexture, nullptr, &lifeRect);
			} else if (!victory && !gameOver) {
				std::cerr << "Warning: lifeTextTexture is null!\n";
			}
		}
	}

	if (Paused) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(renderer, &logicalRect);

		SDL_Texture* pauseTexture =
			TextureManager::RenderText(pauseFont, "Paused", textColor);

		SDL_Rect pauseRect = {1920 / 2 - 200, 1080 / 2 - 50, 400, 100};
		SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);
		SDL_DestroyTexture(pauseTexture);
	}

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderClear(renderer);

	SDL_Rect destRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_RenderCopy(renderer, renderTarget, nullptr, &destRect);
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SaveGame();
	if (!playingReplay) {
		SaveReplay();
	}
	if (lifeTextTexture) SDL_DestroyTexture(lifeTextTexture);
	TTF_CloseFont(font);
	TTF_Quit();
	if (victoryScreen) SDL_DestroyTexture(victoryScreen);
	if (gameOverScreen) SDL_DestroyTexture(gameOverScreen);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
