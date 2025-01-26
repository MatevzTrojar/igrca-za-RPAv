#pragma once

#include "SDL2/SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "clock.hpp"
class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update(Clock *);
	bool running() { return isRunning; }
	void render();
	void clean();

	static SDL_Renderer* renderer;

private:
	bool isRunning = false;
	int cnt = 0;
	SDL_Window* window;
};
