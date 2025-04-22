#include "TextureManager.h"

#include <SDL_ttf.h>

#include <iostream>

#include "Game.h"
#include "SDL_surface.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
	SDL_Surface* tempSurface = IMG_Load(fileName);
	if (!tempSurface) {
		std::cerr << "Failed to load texture: " << fileName << std::endl;
		return nullptr;
	}
	SDL_Texture* tex =
		SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	if (!tex) {
		std::cerr << "Failed to create texture from surface: " << fileName
				  << std::endl;
		return nullptr;
	}

	return tex;
}

SDL_Texture* TextureManager::RenderText(TTF_Font* font, std::string text,
										SDL_Color color) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) return nullptr;

	SDL_Texture* texture =
		SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}
