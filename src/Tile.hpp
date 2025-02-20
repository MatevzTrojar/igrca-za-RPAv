#include <ios>
#include <SDL2/SDL.h>
class Tile{
    public:
        SDL_Rect tile;
        bool isWall,isRendered=false;
        bool IsOffScreen();
};
