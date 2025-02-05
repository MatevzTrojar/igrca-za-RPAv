#pragma once
#include "GameObject.h"

class Scientist :public GameObject{
    public:
     void Update(Clock *,GameObject *);
     Scientist(const char* a,int x,int y):GameObject(a,x,y){

     };

};
