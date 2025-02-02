#include <sys/types.h>
#include "Game.h"
#include "GameObject.h"
#include "SDL_gesture.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/common.hpp"
#include "glm/ext/vector_float2.hpp"



class Bullet : public GameObject {
   public:
	Bullet(const char* textureSheet, int x, int y): GameObject(textureSheet, x, y){
        dest.w = 70;
        dest.h = 70;
	};
    int firerate = 1000;
	~Bullet(){
        Clean();
    };
    Uint32 lastshot = 0;
	void Update(glm::vec2 pos,struct Clock *ura,Uint32 currenttime);
    bool Active = false;
    void Reload();
    void Clean();
};

