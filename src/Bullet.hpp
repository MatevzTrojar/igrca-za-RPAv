#include "Game.h"
#include "GameObject.h"
#include "SDL_render.h"
#include "TextureManager.h"
#include "glm/common.hpp"
#include "glm/ext/vector_float2.hpp"



class Bullet : public GameObject {
   public:
	Bullet(const char* textureSheet, int x, int y): GameObject(textureSheet, x, y){
        dest.w = 20;
        dest.h = 20;
	};
	~Bullet();
	void Update(glm::vec2 pos,struct Clock *);
    bool Active;
};
