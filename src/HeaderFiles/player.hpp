#include <glm/glm.hpp>

#include "GameObject.h"

class Player : public GameObject {
   public:
	glm::vec2 delta;
	Player(const char* textureSheet, int x, int y, int h, int w)
		: GameObject(textureSheet, x, y, h, w) {}
	void Update();
	void Render();

   private:
	bool moving_left = false;
	bool moving_up = false;
	bool moving_down = false;
	bool moving_right = false;
	float deltaX;
	float deltaY;
};
