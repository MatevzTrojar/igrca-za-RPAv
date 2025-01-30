#include "Bullet.hpp"

#include <csignal>
#include <ctime>
#include "GameObject.h"
#include "glm/ext/vector_float2.hpp"



void Bullet::Update(glm::vec2 pos,struct Clock *ura){
    posx+=pos.x *ura->delta;
    posy+=pos.y * ura ->delta;
    dest.x = posx;
    dest.y =posy;
    if(posx > 1920){
        Active = false;
    }
    if(posy>1080)
        Active = false;
    if(posx< 0 || posy < 0)
        Active = false;
}

