#pragma once

#include <sys/ucontext.h>
#include <complex>
#include <ios>
#include <set>
#include "Game.h"
#include "SDL_audio.h"
#include "SDL_mutex.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include <glm/glm.hpp>
#include "glm/detail/qualifier.hpp"
#include "TextureManager.h"
#include <set>
class GameObject {
   public:

       GameObject(){};
	GameObject(const char* textureSheet, int x, int y,int h,int w) ;
//	GameObject(const char* textureSheet, int x, int y, int nFrames, int mSpeed);

	~GameObject() {
        SDL_DestroyTexture(objTexture);
    }
	void Update();
    void FollowPlayer(GameObject* player);
	void Render();
    void Animate();
    void ShelterDetect();
	SDL_Texture* objTexture;
    SDL_Texture* runningTexture= TextureManager::LoadTexture("assets/textures/hampter_running.png");
    SDL_Texture* idleTexture= TextureManager::LoadTexture("assets/textures/hampter.png");
	SDL_Rect srcRect, dest;
	bool moving_left = false, moving_right = false, moving_up = false,
		 moving_down = false;
	bool isFlipped = false;
	bool isAnimated = false;
	int frames = 0;
	int speed = 100;
    bool Collided = false;
	float posx,oldX;
	float posy,oldY; 
    bool Follow = false;
    bool inShelter = false;
int frameDelay = 100;
bool	isRunning;
bool	isIdle;
int	FrameWidth;
int	FrameHeight;
int  lastFrameTime;
int   CurrentFrame;
float ShelterX = 0;
float ShelterY = 0;
    void CollisionDetect(SDL_Rect Border);



};
