#include "GameObject.h"
#include <iostream>
#include <ostream>
#include "SDL_timer.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y) {
	objTexture = TextureManager::LoadTexture(textureSheet);
	SDL_QueryTexture(objTexture, NULL, NULL, &dest.w, &dest.h);
	xpos = x;
	ypos = y;
	isFlipped = false;	// Initially not flipped

}

void GameObject::Update() {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	moving_left = state[SDL_SCANCODE_A];
	moving_up = state[SDL_SCANCODE_W];
	moving_down = state[SDL_SCANCODE_S];
	moving_right = state[SDL_SCANCODE_D];
	if (moving_down) dest.y +=1 ;
	if (moving_right) {
		dest.x += 1;
		isFlipped = true;
	}
	if (moving_up) dest.y -= 1;
	if (moving_left) {
		dest.x -= 1;
		isFlipped = false;
	}
		if (dest.x + dest.w > 1000)
			dest.x = 1000 - dest.w;

		// left boundary
		if (dest.x < 0) dest.x = 0;

		// bottom boundary
		if (dest.y + dest.h > 1000)
			dest.y = 1000 - dest.h;
		// upper boundary
		if (dest.y < 0) dest.y = 0;
       std::cout<<dest.x<<"    "<<dest.y<<std::endl; 
/*	srcRect.h = 100;
	srcRect.w = 100;
	srcRect.x = 0;
	srcRect.y = 0;
    */
	dest.w = 50;
	dest.h = 50;
}

void GameObject::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
