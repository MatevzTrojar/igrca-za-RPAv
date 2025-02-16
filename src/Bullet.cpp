#include "Bullet.hpp"
#include <algorithm>
#include "clock.hpp" 
#include <cmath>
#include <csignal>
#include <ctime>
#include <exception>
#include <fstream>
#include <sstream>
#include <vector>
#include "GameObject.h"
#include "SDL_stdinc.h"
#include "glm/common.hpp"
#include "glm/ext/vector_float2.hpp"
void Bullet::Update( Clock *ura) {
    posx+=pos.x *ura->delta;
    posy+=pos.y * ura ->delta;
    dest.x = posx;
    dest.y =posy;
    if(posx > Game::Camera.x + 1920){
        Active = false;
    }
    if(posy>Game::Camera.y + 1080)
        Active = false;
    if(posx< -Game::Camera.x)
        Active = false;
    if(posy< -Game::Camera.y)
        Active = false;


}

bool Bullet::CollisionDetect(GameObject* other) {
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
        dest.y = other->dest.y - dest.h;
		return true;
	}
    return false;
}

/*void Bullet::Clean() {
    SDL_DestroyTexture(objTexture);
}*/
