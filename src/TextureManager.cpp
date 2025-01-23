#include "TextureManager.h"
#include <system_error>
#include "SDL_image.h"
#include "SDL_render.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer *ren){
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren,tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}
