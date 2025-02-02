#pragma once

#include <complex>

#include "Game.h"

class GameObject {
   public:
	GameObject(const char* textureSheet, int x, int y);
	GameObject(const char* textureSheet, int x, int y, int nFrames, int mSpeed);

	~GameObject() {}

	void Update(Clock*);
	void Render();

	SDL_Texture* objTexture;
	SDL_Rect srcRect, dest;
	bool moving_left = false, moving_right = false, moving_up = false,
		 moving_down = false;
	bool isFlipped = false;
	bool isAnimated = false;
	int frames = 0;
	int speed = 100;
	float posx;
	float posy;
    void Detect();
};
