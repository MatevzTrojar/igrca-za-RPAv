#pragma once

#include <complex>
#include "Game.h"

class GameObject {
   public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject() {}

	void Update(Clock *);
	void Render();
    SDL_Texture* objTexture;
	SDL_Rect srcRect, dest;
	bool moving_left = false, moving_right = false, moving_up = false,
		 moving_down = false;
	bool isFlipped = false;

    private:
	int xpos;
	int ypos;


};
class Scientist :public GameObject{
    public:
     void Update(Clock *,GameObject *);
     Scientist(const char* a,int x,int y):GameObject(a,x,y){

     };

};
