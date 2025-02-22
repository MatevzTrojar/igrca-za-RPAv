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
#include "SDL_hidapi.h"
#include "SDL_platform.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/geometric.hpp"
#include "player.hpp"
#include "Collision.hpp"
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
/*GameObject::GameObject(const char* textureSheet, int x, int y, int nFrames,
					   int mSpeed) {
	isAnimated = true;
	frames = nFrames;
	speed = mSpeed;

	objTexture = TextureManager::LoadTexture(textureSheet);
	SDL_QueryTexture(objTexture, NULL, NULL, &dest.w, &dest.h);
	posx = x;
	posy = y;
	dest.w = 75;
	dest.h = 75;
	isFlipped = false;	// Initially not flipped
	srcRect.h = 100;
	srcRect.w = 100;
	srcRect.x = 0;
	srcRect.y = 1080;
}*/

void GameObject::Update() {
	oldX = posx;
	oldY = posy;
	dest.x = posx - Game::Camera.x;
	dest.y = posy - Game::Camera.y;
}
/*
bool GameObject::CollisionDetect(GameObject* other) {

    
	if (posy + dest.h > other->posy && !(oldY + dest.h > other->posy))
		if (!(posx + dest.w < other->posx ||
			  posx > other->posx + other->dest.w)) {
			posy = other->posy - dest.h;
			return true;
		}
	// spodnja plast
	if (posy < other->posy + other->dest.h &&
		!(oldY < other->posy + other->dest.h))
		if (!(posx + dest.w < other->posx ||
			  posx > other->posx + other->dest.w)) {
			posy = other->posy + other->dest.h;
			return true;
		}
	// leva plast
	if (posx + dest.w > other->posx && !(oldX + dest.w > other->posx))
		if (!(posy + dest.h < other->posy ||
			  posy > other->posy + other->dest.h)) {
			posx = other->posx - dest.w;
			return true;
		}
	// desna plast
	if (posx < other->posx + other->dest.w &&
		!(oldX < other->posx + other->dest.w))
		if (!(posy + dest.h < other->posy ||
			  posy > other->posy + other->dest.h)) {
			posx = other->posx + other->dest.w;
			return true;
		}
	return false;
    
}
*/
GameObject::CollisionSide GameObject::CollisionDetect(SDL_Rect Border) {
    SDL_Rect ObjRect = {(int)posx, (int)posy, dest.w, dest.h};
    
    if (!Collision::AABB(ObjRect, Border)) {
        return  NONE;
    }

    glm::vec2 pos = delta;
    pos.x += posx;
    pos.y += posy;

    glm::vec2 oldPos(oldX, oldY);

    float overlapX = std::min((int)pos.x + dest.w, Border.x + Border.w) - 
                     std::max((int)pos.x, Border.x);
    float overlapY = std::min((int)pos.y + dest.h, Border.y + Border.h) - 
                     std::max((int)pos.y, Border.y);

    if (overlapX <= 0 || overlapY <= 0){
        return NONE;
    }

    const float OFFSET = 0.1f;
    Collided = true;

    if (overlapX < overlapY) { 
        // Horizontal collision (LEFT or RIGHT)
        if (oldPos.x < Border.x) {
         posx = Border.x - dest.w - OFFSET; // Left collision
            delta.x = 0; // Stop horizontal movement
            return LEFT;
        } else {
            posx = Border.x + Border.w + OFFSET; // Right collision
            delta.x = 0; // Stop horizontal movement
            return RIGHT;
        }
    } else { 
        // Vertical collision (TOP or BOTTOM)
        if (oldPos.y < Border.y) {
            posy = Border.y - dest.h - OFFSET; // Top collision
            delta.y = 0; // Stop vertical movement
            return TOP;
        } else {
            posy = Border.y + Border.h + OFFSET; // Bottom collision
            delta.y = 0; // Stop vertical movement
            return BOTTOM;
        }
    }
}
void GameObject::CheckCollisionSide(SDL_Rect Border){
   const float OFFSET = 0.1f;
    CollisionSide side = this->CollisionDetect(Border);
    switch(side){
        case LEFT:
         posx = Border.x - dest.w - OFFSET; // Left collision
            delta.x = 0; // Stop horizontal movementcal movement
            break;
        case RIGHT:
            posx = Border.x + Border.w + OFFSET; // Right collision
            delta.x = 0; // Stop horizontal movement
            break;
        case TOP:
            posy = Border.y - dest.h - OFFSET; // Top collision
            delta.y = 0; // Stop vertical movement
            posy = oldY;
            break;
        case BOTTOM:
            posy = Border.y + Border.h + OFFSET; // Bottom collision
            delta.y = 0; // Stop vertical movement
            break;
        default:
            Collided = false;
            break;
    }

}
void GameObject::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
