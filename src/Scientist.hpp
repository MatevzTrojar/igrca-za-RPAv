#pragma once
#include "Game.h"
#include "GameObject.h"

class Scientist :public GameObject{
    public:
     void Update(Clock *,GameObject *);
     Scientist(const char* a,int x,int y,int h,int w):GameObject(a,x,y,h,w){

     };
 bool CollisionDetect(GameObject* other);
};
