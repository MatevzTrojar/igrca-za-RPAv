#include "Bullet.hpp"
#include <csignal>
#include "GameObject.h"
#include "glm/ext/vector_float2.hpp"
Bullet::~Bullet(){
mTimer = NULL;
delete objTexture;
objTexture = NULL;

};
void Bullet::Fire(glm::vec2 pos){
    dest.x = pos.x;
    dest.y = pos.y;
}
