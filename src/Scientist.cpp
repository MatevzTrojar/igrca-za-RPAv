#include "Scientist.hpp"

#include <cctype>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include "Collision.hpp"
#include "Game.h"
#include "GameObject.h"
#include "clock.hpp"

bool Scientist::HasLineOfSight(GameObject* player, Map* map) {
	const float stepSize = 5.0f;
	glm::vec2 direction =
		glm::normalize(glm::vec2(player->posx - posx, player->posy - posy));
	glm::vec2 currentPos = glm::vec2(posx, posy);

	while (glm::length(currentPos - glm::vec2(player->posx, player->posy)) >
		   stepSize) {
		currentPos += direction * stepSize;

		SDL_Rect CheckRect = {(int)currentPos.x, (int)currentPos.y, dest.w,
							  dest.h};

		for (SDL_Rect& Border : map->Borders) {
			if (Collision::AABB(CheckRect, Border)) {
				return false;
			}
		}
	}
	return true;
}

void Scientist::Update(GameObject* player, Map* map) {
	glm::vec2 move = glm::vec2(player->posx - posx, player->posy - posy);
	float distance = glm::length(move);
	bool CanSeePlayer = (distance < 750) && HasLineOfSight(player, map);

	float movementSpeed = Clock::delta * 0.15f;
	glm::vec2 newPos = glm::vec2(posx, posy);

	if (CanSeePlayer) {
		if (distance > 0.1f) {
			glm::vec2 Finalmove = glm::normalize(move);
			newPos += Finalmove * movementSpeed;
		}
		isFlipped = (move.x > 0);
	} else {
		randomMoveTime -= Clock::delta;

		if (randomMoveTime <= 0) {
			randomMoveTime = (rand() % 3 + 2) * 1000;
			float angle = (rand() % 360) * (3.14159265f / 180.0f);
			randomDirection = glm::vec2(cos(angle), sin(angle));
		}

		newPos += randomDirection * movementSpeed * 0.7f;
	}

	SDL_Rect FutureRect = {(int)newPos.x, (int)newPos.y, dest.w, dest.h};
	bool isColliding = false;

	for (SDL_Rect& Border : map->Borders) {
		if (Collision::AABB(FutureRect, Border)) {
			isColliding = true;
			break;
		}
	}

	if (!isColliding) {
		posx = newPos.x;
		posy = newPos.y;
	} else {
		if (!CanSeePlayer) {
			float angle = (rand() % 360) * (3.14159265f / 180.0f);
			randomDirection = glm::vec2(cos(angle), sin(angle));
			newPos =
				glm::vec2(posx, posy) + randomDirection * movementSpeed * 0.7f;
		} else {
			newPos = glm::vec2(posx, posy) +
					 glm::normalize(glm::vec2(rand() % 2 - 1, rand() % 2 - 1)) *
						 movementSpeed * 0.5f;
		}
	}

	FutureRect = {(int)newPos.x, (int)newPos.y, dest.w, dest.h};
	bool finalCollision = false;
	for (SDL_Rect& Border : map->Borders) {
		if (Collision::AABB(FutureRect, Border)) {
			finalCollision = true;
			break;
		}
	}

	if (!finalCollision) {
		posx = newPos.x;
		posy = newPos.y;
	}

	dest.x = posx - Game::Camera.x;
	dest.y = posy - Game::Camera.y;
}
