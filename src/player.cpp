#include "player.hpp"
#include "Collision.hpp"
#include <algorithm>
#include "glm/detail/qualifier.hpp"
void Player:: Render(){
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);

}


Player::CollisionSide Player::BorderCollisionDetect(SDL_Rect Border) {
    SDL_Rect PlayerRect = {(int)posx, (int)posy, dest.w, dest.h};
    
    if (!Collision::AABB(PlayerRect, Border)) {
        return NONE;
    }

    glm::vec2 pos = delta;
    pos.x += posx;
    pos.y += posy;

    glm::vec2 oldPos(oldX, oldY);

    float overlapX = std::min(pos.x + dest.w, (float)Border.x + Border.w) - 
                     std::max(pos.x, (float)Border.x);
    float overlapY = std::min(pos.y + dest.h, (float)Border.y + Border.h) - 
                     std::max(pos.y, (float)Border.y);

    if (overlapX <= 0 || overlapY <= 0)
        return NONE;

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

    
        /*
        
const float OFFSET = 3;
        glm::vec2 pos = delta;
        pos.x += posx;
        pos.y += posy;
        pos.x = round(pos.x);
        pos.y = round(pos.y);
        std::cout<<"Oldpos: "<<oldX<<" "<<oldY<<" delta: "<<delta.x<<" "<<delta.y<<" vector: "<<pos.x<<" "<<pos.y<<" Position: "<<posx<<" "<<posy<<std::endl;
        	if (pos.y + dest.h > Border.y && !(oldY + dest.h > Border.y))
		if (!(pos.x + dest.w < Border.x ||
			  pos.x > Border.x + Border.w)) {
			posy = Border.y - dest.h- OFFSET;
            delta.y = 0;
			return true;
		}
	// spodnja plast
	if (pos.y < Border.y + Border.h &&
		!(oldY < Border.y + Border.h))
		if (!(pos.x + dest.w < Border.x ||
			  pos.x > Border.x + Border.w)) {
			posy = Border.y + Border.h+ OFFSET;
            delta.y = 0;
			return true;
		}
	// leva plast
	if (pos.x + dest.w > Border.x && !(oldX + dest.w > Border.x))
		if (!(pos.y + dest.h < Border.y ||
			  pos.y > Border.y + Border.h)) {
			posx = Border.x - dest.w- OFFSET;
            delta.x = 0;
			return true;
		}
	// desna plast
	if (pos.x < Border.x + Border.w &&
		!(oldX < Border.x + Border.w))
		if (!(pos.y + dest.h < Border.y ||
			  pos.y > Border.y + Border.h)) {
			posx = Border.x + Border.w+ OFFSET;
            delta.x = 0;
			return true;
		}
	return false;
    */
    

    

