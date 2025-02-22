#include "GameObject.h"
#include "SDL_rwops.h"
#include "glm/integer.hpp"
#include <glm/glm.hpp>
class Player : public GameObject {
    public:
        glm::vec2 delta;
    Player(const char* textureSheet, int x, int y, int h, int w) : GameObject(textureSheet, x, y, h, w) {}
    void Update(Clock* ura) {
    
	const Uint8* state = SDL_GetKeyboardState(NULL);
	moving_left = state[SDL_SCANCODE_A];
	moving_up = state[SDL_SCANCODE_W];
	moving_down = state[SDL_SCANCODE_S];
	moving_right = state[SDL_SCANCODE_D];
	glm::vec2 move;
    oldX = posx;
    oldY = posy;
	move.x = 0;
	move.y = 0;
	if (moving_up) move.y -= 1 * ura->delta * 0.5;
	if (moving_down) move.y += 1 * ura->delta * 0.5;
	if (moving_right) {
		move.x += 1 * ura->delta * 0.5;
		isFlipped = true;
	}
	if (moving_left) {
		move.x -= 1 * ura->delta * 0.5;
		isFlipped = false;
	}
    if(moving_up && moving_right || moving_up && moving_left || moving_down && moving_right || moving_down && moving_left){
        move.x /= 1.414;
       move.y /= 1.414;
    }
    posx += move.x;
    posy += move.y;
    delta.x = posx - oldX;
    delta.y = posy - oldY;
    dest.x = posx-Game::Camera.x;
    dest.y = posy-Game::Camera.y;
    
    if(posx<0){
        posx = 0; 
    }
    if(posy<0){
        posy = 0;
    }
    if(posx>120*32-dest.w){
        posx = 120*32-dest.w;
    }
    if(posy>72*32-dest.h){
        posy = 72*32-dest.h;
    }
    }
    void Render();



};
