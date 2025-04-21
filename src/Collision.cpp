#include "Collision.hpp"
#include "SDL_rect.h"
bool Collision::AABB(SDL_Rect &a, SDL_Rect &b) {
    SDL_Rect *pa = &a;
    SDL_Rect *pb = &b;
    if(!pa || !pb) {
        return false;
    }
	if (a.x + a.w >= b.x &&
        a.x <= b.x + b.w &&
        a.y + a.h >= b.y &&
		a.y <= b.y + b.h)
		return true;
	return false;
}
