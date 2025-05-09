#pragma once
#include <glm/glm.hpp>

#include "Game.h"
#include "GameObject.h"
#include "SDL_keyboard.h"
#include "glm/ext/scalar_int_sized.hpp"
#include "glm/ext/vector_float2.hpp"
class Scientist : public GameObject {
	float randomMoveTime;
	glm::vec2 randomDirection;

   public:
	glm::vec2 delta;
	void Update(GameObject*, Map*);
	Scientist(const char* a, int x, int y, int h, int w)
		: GameObject(a, x, y, h, w) {
		randomMoveTime = (rand() % 3 + 2) * 1000;
		float angle = (rand() % 360) * (3.14159265f / 180.0f);
		randomDirection = glm::vec2(cos(angle), sin(angle));
	};
	bool HasLineOfSight(GameObject* player, Map* map);
};
