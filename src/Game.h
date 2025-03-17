#pragma once

#include "SDL2/SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "SDL_joystick.h"
#include "clock.hpp"
#include "SDL_ttf.h"
class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
    void Overworldinit();
    void Dungeoninit();
	void handleEvents();
	void update(Clock *);
    void Dungeonupdate(Clock *);
    void Overworldupdate(Clock *);

	bool running() { return isRunning; }
	void render();
	void clean();
    void RestartGame();
	static SDL_Renderer* renderer;
    static SDL_Rect Camera;
    static bool overworld;
TTF_Font* font;
SDL_Texture* lifeTextTexture = nullptr;
SDL_Color textColor = {255, 255, 255, 255}; // White
SDL_Texture* victoryScreen = nullptr;
SDL_Texture* gameOverScreen = nullptr;
bool gameOver = false;
bool victory = false;


private:
	bool isRunning = false;
    int level = 0;
	int cnt = 0;
	SDL_Window* window;
};
