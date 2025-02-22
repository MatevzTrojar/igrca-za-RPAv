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


