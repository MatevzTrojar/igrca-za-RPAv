#include "GameObject.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <ostream>
#include <cmath>
#include "Game.h"
#include "SDL_platform.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"

GameObject::GameObject(const char* textureSheet, int x, int y) {
	objTexture = TextureManager::LoadTexture(textureSheet);
	SDL_QueryTexture(objTexture, NULL, NULL, &dest.w, &dest.h);
	dest.x = x;
	dest.y = y;
	isFlipped = false;	// Initially not flipped
}

void GameObject::Update(Clock* ura) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	moving_left = state[SDL_SCANCODE_A];
	moving_up = state[SDL_SCANCODE_W];
	moving_down = state[SDL_SCANCODE_S];
	moving_right = state[SDL_SCANCODE_D];
	if (moving_down) dest.y += 1 * ura->delta;
	if (moving_right) {
		dest.x += 1 * ura->delta;
		isFlipped = true;
	}
	if (moving_up) dest.y -= 1 * ura->delta;
	if (moving_left) {
		dest.x -= 1 * ura->delta;
		isFlipped = false;
	}
	if (dest.x + dest.w > 1920) dest.x = 1920 - dest.w;

	// left boundary
	if (dest.x < 0) dest.x = 0;

	// bottom boundary
	if (dest.y + dest.h > 1080) dest.y = 1080 - dest.h;
	// upper boundary
	if (dest.y < 0) dest.y = 0;
	// std::cout<<dest.x<<"    "<<dest.y<<std::endl;
	/*	srcRect.h = 100;
		srcRect.w = 100;
		srcRect.x = 0;
		srcRect.y = 0;
		*/
	dest.w = 75;
	dest.h = 75;
}

void GameObject::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
void Scientist::Update(Clock* ura, GameObject* player) {
	srand(time(NULL));
	   glm::vec2 move;
	   move.x = player->dest.x - dest.x;
	   move.y= player->dest.y - dest.y;
	   glm::vec2 Finalmove = glm::normalize(move);
	   dest.x += Finalmove.x *ura->delta;
	   dest.y += Finalmove.y * ura -> delta;

	if (dest.x + dest.w > 1920) dest.x = 1920 - dest.w;

	if (dest.x < 0) dest.x = 0;

	if (dest.y + dest.h > 1080) dest.y = 1080 - dest.h;
	if (dest.y < 0) dest.y = 0;
	dest.w = 75;
	dest.h = 75;
}
