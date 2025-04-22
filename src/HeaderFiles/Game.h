#pragma once

#include <vector>

#include "Map.hpp"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "player.hpp"
class Game {
   public:
	struct ReplayData {
		SDL_Point LeftDungeon1;
		SDL_Point LeftDungeon2;
		SDL_Point LeftDungeon3;
	};
	struct DeltaPlayer {
		float posX;
		float posY;
	};

	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
	void Overworldinit(bool isReplay = false);
	void Dungeoninit(bool isReplay = false);
	void update();
	void Dungeonupdate(bool isReplay = false);
	void Overworldupdate(bool isReplay = false);
	bool running(bool ChangeState = false) {
		if (ChangeState) {
			isRunning = !isRunning;
		}
		return isRunning;
	}

	void handleEvents();
	void render();
	void clean();
	void RestartGame();
	void SaveGame();
	void LoadGame();
	void SaveReplay();
	void ContinueGame();

	static SDL_Renderer* renderer;
	static SDL_Rect Camera;
	static bool overworld;
	static int level;
	static std::vector<DeltaPlayer> playerReplayPath;
	static bool gameOver;
	static bool victory;
	static bool MainMenu;
	static bool Paused;
	static bool EndGame;
	static bool recordingReplay;
	static bool playingReplay;
	static int replayFrame;
	static bool Loaded;

	static Map* map;
	static Player* player;
	static SDL_Point LeftDungeon1;
	static SDL_Point LeftDungeon2;
	static SDL_Point LeftDungeon3;

   private:
	int cnt = 0;
	TTF_Font* font;
	SDL_Texture* lifeTextTexture = nullptr;
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Texture* victoryScreen = nullptr;
	SDL_Texture* gameOverScreen = nullptr;
	SDL_Texture* pauseScreen = nullptr;
	SDL_Texture* mainMenuTexture = nullptr;
	SDL_Texture* settingsScreen = nullptr;
	SDL_Texture* leaderboardScreen = nullptr;
	SDL_Texture* EndGameScreen = nullptr;
	SDL_Texture* renderTarget = nullptr;

	SDL_Rect playButton = {447, 169, 500, 127};
	SDL_Rect loadButton = {447, 169 + 203, 500, 127};
	SDL_Rect settingsButton = {447, 169 + 2 * 203, 500, 127};
	SDL_Rect UserButton = {447, 169 + 3 * 203, 500, 127};
	SDL_Rect continueButton = {586, 386, 800, 102};
	SDL_Rect BackToMainMenu = {400, 850, 960, 100};
	SDL_Window* window;

	bool isRunning = false;
};
