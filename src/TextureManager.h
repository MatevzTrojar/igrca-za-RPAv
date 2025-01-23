#pragma once
#include <SDL2/SDL.h>
#include "GameObject.h"
#include "SDL_render.h"
class TextureManager{
    public:
        static SDL_Texture* LoadTexture(const char * filename, SDL_Renderer*ren);
};
