#include "GameObject.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <ostream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include "Game.h"
#include "SDL_platform.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y) {
	objTexture = TextureManager::LoadTexture(textureSheet);
	SDL_QueryTexture(objTexture, NULL, NULL, &dest.w, &dest.h);
	posx = x;
	posy = y;
	isFlipped = false;	// Initially not flipped
/*	srcRect.h = 100;
	srcRect.w = 100;
	srcRect.x = 0;
	srcRect.y = 0;
    */
}
GameObject::GameObject(const char* textureSheet, int x, int y, int nFrames,
					   int mSpeed) {
	isAnimated = true;
	frames = nFrames;
	speed = mSpeed;

	objTexture = TextureManager::LoadTexture(textureSheet);
	SDL_QueryTexture(objTexture, NULL, NULL, &dest.w, &dest.h);
	posx = x; 
	posy = y;
	isFlipped = false;	// Initially not flipped
	srcRect.h = 100;
	srcRect.w = 100;
	srcRect.x = 0;
	srcRect.y = 1080;
}

void GameObject::Update(Clock* ura) {
	if (isAnimated) {
        srcRect.x = srcRect.w * static_cast <int>((SDL_GetTicks() / speed) % frames);
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	moving_left = state[SDL_SCANCODE_A];
	moving_up = state[SDL_SCANCODE_W];
	moving_down = state[SDL_SCANCODE_S];
	moving_right = state[SDL_SCANCODE_D];
	if (moving_down) posy += 1 * ura->delta*0.8;
	if (moving_right) {
		posx += 1 * ura->delta*0.8;
		isFlipped = true;
	}
	if (moving_up) posy -= 1 * ura->delta*0.8;
	if (moving_left) {
		posx -= 1 * ura->delta*0.8;
		isFlipped = false;
	}
	if (posx + dest.w > 1920) posx = 1920 - dest.w;

	// left boundary
	if (posx < 0) posx = 0;

	// bottom boundary
	if (posy + dest.h > 1080) posy = 1080 - dest.h;
	// upper boundary
	if (posy < 0) posy = 0;
	dest.w = 75;
	dest.h = 75;
    dest.x=posx;
    dest.y=posy;
}

void GameObject::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
