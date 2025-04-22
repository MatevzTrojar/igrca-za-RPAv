#pragma once
#include <SDL2/SDL.h>

#include <cstdlib>

class Collision {
   public:
	static bool AABB(SDL_Rect &a, SDL_Rect &b);
};
