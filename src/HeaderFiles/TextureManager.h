#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>

class TextureManager {
   public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static SDL_Texture* RenderText(TTF_Font*, std::string text,
								   SDL_Color color);
};
