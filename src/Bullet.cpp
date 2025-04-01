#include "Bullet.hpp"
#include <algorithm>
#include "Game.h"
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
void Bullet::Update(GameObject *player) {
    posx+=pos.x *Clock::delta;
    posy+=pos.y * Clock::delta;
    dest.x = posx - Game::Camera.x; 
    dest.y =posy - Game::Camera.y;
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


