#include "Scientist.hpp"

#include <cctype>
#include <experimental/filesystem>
#include <functional>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <type_traits>

#include "Game.h"
#include "GameObject.h"
#include "SDL_render.h"

void Scientist::Update(Clock* ura, GameObject* player) {
	glm::vec2 move;
    oldX = posx;
    oldY = posy;
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
    delta.x = posx - oldX;
    delta.y = posy - oldY;  
    dest.x = posx-Game::Camera.x;
    dest.y = posy-Game::Camera.y;
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
    bool Scientist:: BorderCollisionDetect(SDL_Rect Border){
        	if (posy + dest.h > Border.y && !(oldY + dest.h > Border.y))
		if (!(posx + dest.w < Border.x ||
			  posx > Border.x + Border.w)) {
			posy = Border.y - dest.h;
			return true;
		}
	// spodnja plast
	if (posy < Border.y + Border.h &&
		!(oldY < Border.y + Border.h))
		if (!(posx + dest.w < Border.x ||
			  posx > Border.x + Border.w)) {
			posy = Border.y + Border.h;
			return true;
		}
	// leva plast
	if (posx + dest.w > Border.x && !(oldX + dest.w > Border.x))
		if (!(posy + dest.h < Border.y ||
			  posy > Border.y + Border.h)) {
			posx = Border.x - dest.w;
			return true;
		}
	// desna p
    // last
	if (posx < Border.x + Border.w &&
		!(oldX < Border.x + Border.w))
		if (!(posy + dest.h < Border.y ||
			  posy > Border.y + Border.h)) {
			posx = Border.x + Border.w;
			return true;
		}
	return false;
    }
