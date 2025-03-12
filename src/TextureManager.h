#pragma once

#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName);
    static SDL_Texture* RenderText(TTF_Font*, std::string text, SDL_Color color);
};
