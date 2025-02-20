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
bool GameObject::CollisionDetect(GameObject* other) {
	    right ={dest.x + dest.w + 1, dest.y, 1, dest.h};
		left= {dest.x - 1, dest.y, 1, dest.h};
        up={dest.x, dest.y - 1, dest.w, 1};
		down={dest.x, dest.y + dest.h + 1, dest.w, 1};
       if(other->isWall){
           if(SDL_HasIntersection(&dest, &other->dest)){
               if(SDL_HasIntersection(&right, &other->dest)){
                   posx = other->posx - dest.w;
                   return true;
               }
               else if(SDL_HasIntersection(&left, &other->dest)){
                   posx = other->posx + other->dest.w;
                   return true;
               }
               else if(SDL_HasIntersection(&up, &other->dest)){
                   posy = other->posy + other->dest.h;
                   return true;
               }
               else if(SDL_HasIntersection(&down, &other->dest)){
                   posy = other->posy - dest.h;
                   return true;
               }
           }
       } 
	/*
	if(other->isWall){
	if (posy + dest.h > other->posy && !(oldY + dest.h > other->posy))
		if (!(posx + dest.w < other->posx || posx > other->posx +
	other->dest.w)){ posy = other->posy - dest.h; return true;
		}
	// spodnja plast
	if (posy < other->posy + other->dest.h && !(oldY < other->posy +
	other->dest.h)) if (!(posx + dest.w < other->posx || posx > other->posx +
	other->dest.w)){ posy = other->posy + other->dest.h; return true;
		}
	// leva plast
	if (posx + dest.w > other->posx && !(oldX + dest.w > other->posx))
		if (!(posy + dest.h < other->posy || posy > other->posy +
	other->dest.h)){ posx = other->posx - dest.w; return true;
		}
	// desna plast
	if (posx < other->posx + other->dest.w && !(oldX < other->posx +
	other->dest.w)) if (!(posy + dest.h < other->posy || posy > other->posy +
	other->dest.h)){ posx = other->posx + other->dest.w; return true;
		}
	}
	return false;
	*/
       return false;
}
void GameObject::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
