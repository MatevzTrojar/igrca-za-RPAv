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
    if(posx > 1920){
        Active = false;
    }
    if(posy>1080)
        Active = false;
    if(posx< 0)
        Active = false;
    if(posy<0)
        Active = false;

}



/*void Bullet::Clean() {
    SDL_DestroyTexture(objTexture);
}*/
