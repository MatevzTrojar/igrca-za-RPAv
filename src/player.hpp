#include "GameObject.h"
#include "SDL_rwops.h"
#include "glm/integer.hpp"
#include <glm/glm.hpp>
class Player : public GameObject {
    public:
    float deltaX;
        float deltaY;
        glm::vec2 delta;
    Player(const char* textureSheet, int x, int y, int h, int w) : GameObject(textureSheet, x, y, h, w) {}
    void Update(); 
    void Render();



};
