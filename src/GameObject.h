#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_surface.h"
class GameObject{
    public:
SDL_Rect dest;
SDL_Rect source;
SDL_Texture* tex;
bool moving_left = false, moving_right = false, moving_up = false, moving_down = false;
SDL_Surface *surface;
};
