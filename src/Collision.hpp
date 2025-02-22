#pragma once
#include <cstdlib>
#include "SDL_audio.h"
#include <SDL2/SDL.h>
class Collision
{
public:
    static bool AABB(SDL_Rect &a, SDL_Rect &b);
};
