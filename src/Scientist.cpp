#include "Scientist.hpp"

#include <cctype>
#include <cstdlib>	// For rand()
#include <ctime>	// For seeding rand()
#include <experimental/filesystem>
#include <functional>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <type_traits>

#include "Collision.hpp"
#include "Game.h"
#include "GameObject.h"
#include "SDL_render.h"
bool Scientist::HasLineOfSight(GameObject* player, Map* map) {
// Step size for ray traversal
    const float stepSize = 5.0f;  

    glm::vec2 direction = glm::normalize(glm::vec2(player->posx - posx, player->posy - posy));
    glm::vec2 currentPos = glm::vec2(posx, posy);

    while (glm::length(currentPos - glm::vec2(player->posx, player->posy)) > stepSize) {
        currentPos += direction * stepSize;

        SDL_Rect CheckRect = {
            (int)std::floor(currentPos.x),
            (int)std::floor(currentPos.y),
            dest.w, dest.h};

        for (SDL_Rect Border : map->Borders) {
            if (Collision::AABB(CheckRect, Border)) {
                return false;  // Wall detected, player is hidden
            }
        }
    }

    return true;  // No wall blocking the path

}
void Scientist::Update(Clock* ura, GameObject* player, Map* map) {
	glm::vec2 move;
	oldX = posx;
	oldY = posy;
	move.x = player->posx - posx;
	move.y = player->posy - posy;
	float distance = glm::length(move);
    bool CanSeePlayer = HasLineOfSight(player, map) && distance < 750;
	if (CanSeePlayer) {
		// Chase the player
		glm::vec2 Finalmove = glm::normalize(move);

		if (Finalmove.x > 0) {
			isFlipped = true;
		} else if (Finalmove.x < 0) {
			isFlipped = false;
		}

		GameObject::posx += Finalmove.x * ura->delta * 0.2;
		GameObject::posy += Finalmove.y * ura->delta * 0.2;
	} else {
		// Move randomly
		randomMoveTime -= ura->delta;

		if (randomMoveTime <= 0) {
			// Pick a new random direction every 2-5 seconds
			randomMoveTime = (rand() % 3 + 2) * 1000;  // 2-5 seconds
			float angle = (rand() % 360) *
						  (3.14159265f / 180.0f);  // Convert degrees to radians
			randomDirection = glm::vec2(cos(angle), sin(angle));
			SDL_Rect NextDest = {
				(int)std::floor(posx + randomDirection.x * ura->delta * 0.1),
				(int)std::floor(posy + randomDirection.y * ura->delta * 0.1),
				dest.w, dest.h};
			for (SDL_Rect Border : map->Borders) {
				if (Collision::AABB(NextDest, Border)) {
                    randomMoveTime = 0; 
                    break;
				}
			}
		}

		GameObject::posx += randomDirection.x * ura->delta * 0.1;
		GameObject::posy += randomDirection.y * ura->delta * 0.1;
	}

	dest.x = posx - Game::Camera.x;
	dest.y = posy - Game::Camera.y;
}
