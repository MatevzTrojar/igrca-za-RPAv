#include "GameObject.h"
#include <endian.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <ostream>
#include <set>
#include <system_error>

#include "Bullet.hpp"
#include "Game.h"
#include "Mouse.hpp"
#include "SDL_platform.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/geometric.hpp"
GameObject::GameObject(const char* textureSheet, int x, int y) {
	objTexture = TextureManager::LoadTexture(textureSheet);
	SDL_QueryTexture(objTexture, NULL, NULL, &dest.w, &dest.h);
	posx = x;
	posy = y;
	isFlipped = false;	// Initially not flipped
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
	Mouse mouse;
	if (isAnimated) {
		srcRect.x =
			srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	moving_left = state[SDL_SCANCODE_A];
	moving_up = state[SDL_SCANCODE_W];
	moving_down = state[SDL_SCANCODE_S];
	moving_right = state[SDL_SCANCODE_D];
	glm::vec2 move;
	move.x = 0;
	move.y = 0;
	glm::vec2 FinalMove;
	if (moving_up) move.y -= 1 * ura->delta * 0.6;
	if (moving_down) move.y += 1 * ura->delta * 0.6;
	if (moving_right) {
		move.x += 1 * ura->delta * 0.6;
		isFlipped = true;
	}
	if (moving_left) {
		move.x -= 1 * ura->delta * 0.6;
		isFlipped = false;
	}
	FinalMove = move;
	posx += FinalMove.x;
	posy += FinalMove.y;

	if (posx + dest.w > 1920) posx = 1920 - dest.w;

	// left boundary
	if (posx < 0) posx = 0;

	// bottom boundary
	if (posy + dest.h > 1080) posy = 1080 - dest.h;
	// upper boundary
	if (posy < 0) posy = 0;
	dest.w = 75;
	dest.h = 75;
	dest.x = posx;
	dest.y = posy;
}
bool GameObject::CollisionDetect(GameObject* other) {
	// right collision
	if (dest.x + dest.w > other->dest.x &&
		dest.x < other->dest.x + other->dest.w &&
		dest.y + dest.h > other->dest.y &&
		dest.y < other->dest.y + other->dest.h) {
        dest.x = other->dest.x - dest.w;
		return true;
	}
	// left collision
	if (other->dest.x + other->dest.w > dest.x &&
		other->dest.x < dest.x + dest.w &&
		other->dest.y + other->dest.h > dest.y &&
		other->dest.y < dest.y + dest.h) {
        dest.x = other->dest.x + other->dest.w;
		return true;
	}
	// upper collision
	if (dest.x + dest.w > other->dest.x &&
		dest.x < other->dest.x + other->dest.w &&
		dest.y < other->dest.y + other->dest.h &&
		dest.y + dest.h > other->dest.y) {
        dest.y = other->dest.y + other->dest.h;
		return true;
	}
	// down collision
	if (other->dest.x + other->dest.w > dest.x &&
		other->dest.x < dest.x + dest.w && other->dest.y < dest.y + dest.h &&
		other->dest.y + other->dest.h > dest.y) {
        dest.y = other->dest.y - dest.h;
		return true;
	}
    return false;
}


void GameObject::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
