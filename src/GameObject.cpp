
#include "GameObject.h"

#include <endian.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include "Bullet.hpp"
#include "Collision.hpp"
#include "Game.h"
#include "Mouse.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "clock.hpp"
#include "glm/geometric.hpp"

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
	isRunning = false;
	isIdle = true;
	FrameWidth = w;
	FrameHeight = h;
	lastFrameTime = 0;
	CurrentFrame = 0;

	srcRect = {0, 0, FrameWidth, FrameHeight};
}

void GameObject::Update() {
	oldX = posx;
	oldY = posy;
	dest.x = posx - Game::Camera.x;
	dest.y = posy - Game::Camera.y;
	Animate();
}

void GameObject::FollowPlayer(GameObject* player) {
	if (inShelter) {
		isIdle = true;
		isRunning = false;
		objTexture = idleTexture;
		return;
	}

	isAnimated = true;
	glm::vec2 move = glm::vec2(player->posx - posx, player->posy - posy);
	float distance = glm::length(move);

	if (distance > 0) {
		move = glm::normalize(move);
	}

	SDL_Rect AnimalDest = {(int)posx, (int)posy, dest.w, dest.h};
	SDL_Rect PlayerDest = {(int)player->posx, (int)player->posy, player->dest.w,
						   player->dest.h};

	if (Collision::AABB(AnimalDest, PlayerDest)) {
		isIdle = true;
		isRunning = false;
		objTexture = idleTexture;
	} else {
		isIdle = false;
		isRunning = true;
		objTexture = runningTexture;
		posx += move.x * 0.2f * Clock::delta;
		posy += move.y * 0.2f * Clock::delta;
	}

	dest.x = posx - Game::Camera.x;
	dest.y = posy - Game::Camera.y;
}
void GameObject::ResetAnimation() {
	this->CurrentFrame = 0;
}
void GameObject::Animate() {
	Uint32 currentTime = SDL_GetTicks();

	if (currentTime - lastFrameTime > frameDelay) {
		lastFrameTime = currentTime;

		if (isRunning || isIdle) {
			CurrentFrame++;
			if (CurrentFrame >= 7) CurrentFrame = 0;

			srcRect.x = CurrentFrame * FrameWidth;
		}
	}
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
		if (oldPos.x < Border.x) {
			posx -= overlapX;
		} else {
			posx += overlapX;
		}
	} else {
		if (oldPos.y < Border.y) {
			posy -= overlapY;
		} else {
			posy += overlapY;
		}
	}
}

void GameObject::ShelterDetect() {
	SDL_Rect Shelter = {150, 480, 494 - 127, 120};
	SDL_Rect tempDest = {(int)posx, (int)posy, dest.w, dest.h};
	SDL_Rect* ShelterPtr = &Shelter;
	SDL_Rect* tempDestPtr = &tempDest;
	if (SDL_HasIntersection(ShelterPtr, tempDestPtr) && Game::overworld) {
		inShelter = true;

	} else {
		inShelter = false;
	}
}

void GameObject::Render() {
	if (!isAnimated) {
		if (!isFlipped)
			SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
							 SDL_FLIP_HORIZONTAL);

		else
			SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
	} else {
		SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &dest);
	}
}
