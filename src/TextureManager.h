#pragma once

#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName);
//	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
    static SDL_Texture* LoadBeatmapTexture(const char* fileName);
    static SDL_Texture* RenderText(TTF_Font*, std::string text, SDL_Color color);
};
