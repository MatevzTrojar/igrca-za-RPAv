#pragma once

#include <sys/ucontext.h>
#include <complex>
#include <iomanip>
#include <set>
#include "Game.h"
#include "SDL_render.h"
#include <set>
class GameObject {
   public:
	GameObject(const char* textureSheet, int x, int y);
	GameObject(const char* textureSheet, int x, int y, int nFrames, int mSpeed);

	~GameObject() {
        SDL_DestroyTexture(objTexture);
    }

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
	float posx,oldX;
	float posy,oldY;
    bool CollisionDetect(GameObject* other);


};
