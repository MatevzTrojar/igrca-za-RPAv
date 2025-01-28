#ifndef _BULLET_HPP
#define _BULLET_HPP
#include "GameObject.h"
#include "SDL_render.h"
#include "TextureManager.h"
#include "glm/ext/vector_float2.hpp"

#endif
class Bullet : public GameObject {
   private:
	const int OFFSCREEN_BUFFER = 10;
	Clock *mTimer;
	float mSpeed;
    bool Active;
   public:
	Bullet(const char* textureSheet, int x, int y): GameObject(textureSheet, x, y) {
        mTimer->tick();
        mSpeed = 500.0;
        posx =0;
        posy = 0;
        Reload();
	};
	~Bullet();
	void Fire(glm::vec2 pos);
	void Reload();
};
