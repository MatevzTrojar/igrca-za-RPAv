#include "Game.h"

#include <SDL_ttf.h>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <glm/glm.hpp>
#include <iomanip>
#include <ios>
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
#include "SDL_mutex.h"
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
bool Game::MainMenu = true;
bool Game::EndGame = false;
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
    EndGame = false;
    Overworldinit();
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
    if (overworld)
        data.PetCount = (Level1Pet != nullptr) + (Level2Pet != nullptr) +
                        (Level3Pet != nullptr);
    else
        data.PetCount = 0;

    SaveFile.write(reinterpret_cast<const char*>(&data), sizeof(SaveData));
    if (overworld) {
        SaveObjectData saveData;
        if (Level1Pet) {
            // GetSaveData() should now also return shelter coordinates.
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
    }

    SaveFile.close();
    std::cout << "Level: " << level << std::endl;
    std::cout << "Life: " << life << std::endl;
    std::cout << "Overworld: " << overworld << std::endl;
    std::cout << "Victory: " << victory << std::endl;
    std::cout << "GameOver: " << gameOver << std::endl;
    std::cout << "EndGame: " << EndGame << std::endl;
}

bool Game::Loaded = false;
void Game::LoadGame() {
    Loaded = true;
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
    
    if (overworld) {
        // Updated lambda now creates the pet using saved coordinates.
        auto loadPet = [&](GameObject*& pet, bool followState, const char* texture) {
            SaveObjectData petData;
            if (!LoadFile.read(reinterpret_cast<char*>(&petData), sizeof(petData))) {
                std::cerr << "Failed to read pet data!" << std::endl;
                return;
            }
            // Create the pet using the saved position from petData.
            pet = new GameObject(texture, petData.posx, petData.posy, 64, 64);
            pet->SetSaveData(petData); // This should restore position, shelterX, shelterY, etc.
            pet->Follow = followState;
            pet->ResetAnimation();
        };

        if (data.PetCount >= 1)
            loadPet(Level1Pet, data.Follow1, "assets/textures/animated_pet.png");
        if (data.PetCount >= 2)
            loadPet(Level2Pet, data.Follow2, "assets/textures/animated_pet.png");
        if (data.PetCount >= 3)
            loadPet(Level3Pet, data.Follow3, "assets/textures/animated_pet.png");
    }
    LoadFile.close();
    std::cout << "Level: " << level << std::endl;
    std::cout << "Life: " << life << std::endl;
    std::cout << "Overworld: " << overworld << std::endl;
    std::cout << "Victory: " << victory << std::endl;
    std::cout << "GameOver: " << gameOver << std::endl;
    std::cout << "EndGame: " << EndGame << std::endl;
    if (overworld) {
        Overworldinit();
    } else {
        Dungeoninit();
    }
    Loaded = false;
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
    TextureManager::RenderText(font, "Hitpoints: " + std::to_string(life), textColor);
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
    EndGameScreen = TextureManager::LoadTexture("assets/textures/EndGame.png");
    gameOverScreen = TextureManager::LoadTexture("assets/textures/gameover.png");
    mainMenuTexture = TextureManager::LoadTexture("assets/textures/MainMenu.png");
    EndGameScreen = TextureManager::LoadTexture("assets/textures/EndGame.PNG");
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

        // Use the animated texture if needed.
        Level1Pet = new GameObject("assets/textures/cage.png", 300, 300, 64, 64);
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
        Level2Pet = new GameObject("assets/textures/cage.png", 71 * 32, 58 * 32, 64, 64);
        Level2Pet->ResetAnimation();
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
        Level3Pet = new GameObject("assets/textures/cage.png", 71 * 32, 65 * 32, 64, 64);
        Level3Pet->ResetAnimation();
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
        // Reposition player regardless of loaded state.
        player->posx = 30 * 32;
        player->posy = 25 * 32;
        map->Bitmap = TextureManager::LoadTexture("assets/textures/overworld.xcf");
        map->checkOverWorld();
        map->AssignRand();
        map->LoadMap();
        map->AssignBorders();

        // Reposition pets if they were not loaded from file.
        if (Level1Pet && level == 1 && !Loaded) {
            Level1Pet->posx = 33 * 32;
            Level1Pet->posy = 19 * 32;
            Level1Pet->ResetAnimation();
        }
        if (Level2Pet && level == 2 && !Loaded) {
            Level2Pet->posx = 33 * 32;
            Level2Pet->posy = 19 * 32;
            Level2Pet->ResetAnimation();
        }
        if (Level3Pet && level == 3 && !Loaded) {
            Level3Pet->posx = 33 * 32;
            Level3Pet->posy = 19 * 32;
            Level3Pet->ResetAnimation();
        }
        if(level == 2 && Loaded){
            Level1Pet->posx = Level1Pet->ShelterX;
            Level1Pet->posy = Level1Pet->ShelterY;
            Level1Pet->objTexture = TextureManager::LoadTexture("assets/textures/hampter.png");
            Level1Pet->FollowPlayer(player);
            if(Level2Pet){
                Level2Pet->posx = Level2Pet->ShelterX;
                Level2Pet->posy = Level2Pet->ShelterY;
                Level2Pet->objTexture = TextureManager::LoadTexture("assets/textures/hampter.png");
                Level2Pet->FollowPlayer(player);
            }
        }
        if(level == 3 && Loaded){
            Level1Pet->posx = Level1Pet->ShelterX;
            Level1Pet->posy = Level1Pet->ShelterY;
            Level1Pet->objTexture = TextureManager::LoadTexture("assets/textures/hampter.png");
            Level1Pet->FollowPlayer(player);
            Level2Pet->posx = Level2Pet->ShelterX;
            Level2Pet->posy = Level2Pet->ShelterY;
            Level2Pet->objTexture = TextureManager::LoadTexture("assets/textures/hampter.png");
            Level2Pet->FollowPlayer(player);
            if(Level3Pet){
                Level3Pet->posx = Level3Pet->ShelterX;
                Level3Pet->posy = Level3Pet->ShelterY;
                Level3Pet->objTexture = TextureManager::LoadTexture("assets/textures/hampter.png");
                Level3Pet->FollowPlayer(player);
            }
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
        case SDL_MOUSEBUTTONDOWN: {
            SDL_Rect mouseRect = {(int)mouse.xpos, (int)mouse.ypos, 1, 1};
            if (MainMenu) {
                // Play button clicked
                if (Collision::AABB(playButton, mouseRect)) {
                    MainMenu = false;  // Start the game
                    RestartGame();
                }
                // Load button clicked
                else if (Collision::AABB(loadButton, mouseRect)) {
                    LoadGame();
                    MainMenu = false;  // Start the game
                } else if (Collision::AABB(settingsButton, mouseRect)) {
                    // settings
                } else if (Collision::AABB(leaderboardButton, mouseRect)) {
                    // leaderboard
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
        Bullet* bullet = new Bullet("assets/textures/bullet.png", player->posx + 10,
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
            SDL_Rect tempBullet{static_cast<int>(bullet->posx), static_cast<int>(bullet->posy),
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
        SDL_Rect tempPlayer{static_cast<int>(player->posx), static_cast<int>(player->posy),
                            player->dest.w, player->dest.h};
        SDL_Rect tempScientist{static_cast<int>(scientist->posx), static_cast<int>(scientist->posy),
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
    if (Level1Pet && Level1Pet->inShelter && Level2Pet &&
        Level2Pet->inShelter && Level3Pet && Level3Pet->inShelter) {
        EndGame = true;
    }
    if (level == 2) {
        // Reposition using the loaded shelter coordinates
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
    SDL_Rect tempPlayer{static_cast<int>(player->posx), static_cast<int>(player->posy),
                        player->dest.w, player->dest.h};
    Tile* playerTile = &map->tile[static_cast<int>(player->posx) / 32][static_cast<int>(player->posy) / 32];
    if (Collision::AABB(tempPlayer, playerTile->dest) &&
        ((map->map[static_cast<int>(player->posx) / 32][static_cast<int>(player->posy) / 32] == '4' ||
          map->map[static_cast<int>(player->posx) / 32][static_cast<int>(player->posy) / 32] == '7') &&
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
    if (MainMenu) {
        // Render the main menu background
        SDL_RenderCopy(renderer, mainMenuTexture, nullptr, nullptr);
    } else if (EndGame) {
        SDL_Rect ContinueButton = {406, 852, 947, 100},
                 mouseRect = {(int)mouse.xpos, (int)mouse.ypos, 1, 1};
        if (Collision::AABB(ContinueButton, mouseRect) && mouse.click) {
            RestartGame();
            MainMenu = true;
        }
        SDL_RenderCopy(renderer, EndGameScreen, nullptr, &Fullscreen);
    } else if (victory) {
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
    } else {
        // Render overworld or dungeon objects.
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

        for (Scientist* scientist : scientists) {
            if (scientist) {
                scientist->Render();
            } else {
                std::cerr << "Error: scientist is null!\n";
            }
        }

        for (Bullet* bullet : bullets) {
            if (bullet && bullet->Active) {
                bullet->Render();
            } else if (bullet == nullptr) {
                std::cerr << "Error: bullet is null!\n";
            }
        }

        if (!victory && !gameOver && lifeTextTexture) {
            SDL_Rect lifeRect = {1720, 20, 180, 50};
            SDL_RenderCopy(renderer, lifeTextTexture, nullptr, &lifeRect);
        } else if (!victory && !gameOver) {
            std::cerr << "Warning: lifeTextTexture is null!\n";
        }
    }
    if (Paused) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(renderer, &Fullscreen);
        SDL_Texture* pauseTexture = TextureManager::RenderText(pauseFont, "Paused", textColor);
        SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 50, 400, 100};
        SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);
        SDL_DestroyTexture(pauseTexture);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SaveGame();
    if (lifeTextTexture) SDL_DestroyTexture(lifeTextTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    if (victoryScreen) SDL_DestroyTexture(victoryScreen);
    if (gameOverScreen) SDL_DestroyTexture(gameOverScreen);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
