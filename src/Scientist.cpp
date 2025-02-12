#include "Scientist.hpp"

#include <cctype>
#include <experimental/filesystem>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <type_traits>

#include "GameObject.h"

void Scientist::Update(Clock* ura, GameObject* player) {
	glm::vec2 move;
	move.x = player->posx - posx;
	move.y = player->posy - posy;
	glm::vec2 Finalmove = glm::normalize(move);
    if(Finalmove.x > 0){
        isFlipped = true;
    }
    if(Finalmove.x < 0){
        isFlipped = false;
    }
	GameObject::posx += Finalmove.x * ura->delta * 0.2;
	GameObject::posy += Finalmove.y * ura->delta * 0.2;
	dest.x = posx;
	dest.y = posy;

	if (dest.x + dest.w > 1920) dest.x = 1920 - dest.w;

	if (dest.x < 0) dest.x = 0;

	if (dest.y + dest.h > 1080) dest.y = 1080 - dest.h;
	if (dest.y < 0) dest.y = 0;
	dest.w = 75;
	dest.h = 75;
}
bool Scientist::CollisionDetect(GameObject* other) {
// right collision
if (dest.x + dest.w > other->dest.x &&
    dest.x < other->dest.x + other->dest.w &&
    dest.y + dest.h > other->dest.y &&
    dest.y < other->dest.y + other->dest.h) {
    return true;
}
// left collision
if (other->dest.x + other->dest.w > dest.x &&
    other->dest.x < dest.x + dest.w &&
    other->dest.y + other->dest.h > dest.y &&
    other->dest.y < dest.y + dest.h) {
    return true;
}
// upper collision
if (dest.x + dest.w > other->dest.x &&
    dest.x < other->dest.x + other->dest.w &&
    dest.y < other->dest.y + other->dest.h &&
    dest.y + dest.h > other->dest.y) {
    return true;
}
// down collision
if (other->dest.x + other->dest.w > dest.x &&
    other->dest.x < dest.x + dest.w && other->dest.y < dest.y + dest.h &&
    other->dest.y + other->dest.h > dest.y) {
    return true;
}
return false;
}
