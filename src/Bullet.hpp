#include <sys/types.h>
#include <cstdio>
#include <vector>
#include "Game.h"
#include "GameObject.h"
#include "SDL_gesture.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "glm/common.hpp"
#include "glm/ext/vector_float2.hpp"
#include <set>
#include "Scientist.hpp"

class Bullet : public GameObject {
   public:
	Bullet(const char* textureSheet, int x, int y,int h ,int w): GameObject(textureSheet, x, y, h, w){                                                                        

	};
    glm::vec2 pos;
    int firerate = 3000;
	~Bullet(){
    //    Clean();
    };
	void Update(Clock *ura);
    bool Active = false;
    };

