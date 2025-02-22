#include "player.hpp"
#include "Collision.hpp"
#include <algorithm>
#include "glm/detail/qualifier.hpp"
void Player:: Render(){
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);

}



    

    

