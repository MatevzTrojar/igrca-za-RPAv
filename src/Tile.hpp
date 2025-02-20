#include <ios>
#include <SDL2/SDL.h>
#include "GameObject.h"
class Tile :public GameObject{
    public:
        bool isRendered=false;
        bool IsOffScreen();
};
