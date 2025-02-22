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

