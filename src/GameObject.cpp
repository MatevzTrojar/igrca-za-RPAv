
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
#include "Collision.hpp"
#include "Game.h"
#include "Mouse.hpp"
#include "SDL_hidapi.h"
#include "SDL_platform.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/geometric.hpp"
#include "player.hpp"
GameObject::GameObject(const char* textureSheet, int x, int y, int h, int w) {
	objTexture = TextureManager::LoadTexture(textureSheet);
	SDL_QueryTexture(objTexture, NULL, NULL, &dest.w, &dest.h);
	posx = x;
	posy = y;
	isFlipped = false;
	dest.w = w;
	dest.y = posy;
	dest.x = posx;
	dest.h = h;
}

void GameObject::Update() {
	oldX = posx;
	oldY = posy;
	dest.x = posx - Game::Camera.x;
	dest.y = posy - Game::Camera.y;
}

void GameObject::CollisionDetect(SDL_Rect Border) {
	SDL_Rect ObjRect = {(int)std::floor(posx), (int)std::floor(posy), 32, 32};
	glm::vec2 oldPos(oldX, oldY);
	float overlapX = std::min((int)posx + dest.w, Border.x + Border.w) -
					 std::max((int)posx, Border.x);
	float overlapY = std::min((int)posy + dest.h, Border.y + Border.h) -
					 std::max((int)posy, Border.y);

	if (overlapX <= 0 || overlapY <= 0) {
		return;
	}

	Collided = true;

	if (overlapX < overlapY) {
		// Horizontal collision (LEFT or RIGHT)
		if (oldPos.x < Border.x) {
			posx -= overlapX;  // Push player back from left collision
		} else {
			posx += overlapX;  // Push player back from right collision
		}
	} else {
		// Vertical collision (TOP or BOTTOM)
		if (oldPos.y < Border.y) {
			posy -= overlapY;  // Push player back from top collision
		} else {
			posy += overlapY;  // Push player back from bottom collision
		}
	}
}

void GameObject::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
