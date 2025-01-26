#pragma once

#include "Game.h"

class GameObject {
   public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject() {}

	void Update();
	void Render();

    private:
	int xpos;
	int ypos;
	bool isFlipped = false;
	bool moving_left = false, moving_right = false, moving_up = false,
		 moving_down = false;
	SDL_Texture* objTexture;
	SDL_Rect srcRect, dest;
};
