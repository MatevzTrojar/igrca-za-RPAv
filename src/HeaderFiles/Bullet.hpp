#include <sys/types.h>

#include <cstdio>

#include "GameObject.h"

class Bullet : public GameObject {
   public:
	Bullet(const char* textureSheet, int x, int y, int h, int w)
		: GameObject(textureSheet, x, y, h, w) {

		  };
	glm::vec2 pos;
	~Bullet() {};
	void Update(GameObject* player);
	bool Active = false;
};
