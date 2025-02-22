#pragma once
#include "Game.h"
#include "GameObject.h"
#include "SDL_keyboard.h"
#include <glm/glm.hpp>

class Scientist :public GameObject{
    public:
        glm::vec2 delta;
     void Update(Clock *,GameObject *);
     Scientist(const char* a,int x,int y,int h,int w):GameObject(a,x,y,h,w){

     };
 bool CollisionDetect(GameObject* other);
 bool BorderCollisionDetect(SDL_Rect Border);
};
