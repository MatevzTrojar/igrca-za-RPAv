#include <SDL2/SDL.h>

class Tile{
   public:
    SDL_Rect dest;
    float posx,posy;
	bool isRendered = false;
//	bool IsOffScreen();
	bool isWall = false;
	bool Used = false;

};
