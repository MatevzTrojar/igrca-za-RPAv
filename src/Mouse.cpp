#include "Mouse.hpp"

#include "Game.h"
#include "SDL_events.h"
void Mouse::Update(SDL_Event event) {
	if (SDL_MOUSEMOTION == event.type) {
		SDL_GetMouseState(&x, &y);
	}

	if (SDL_MOUSEBUTTONDOWN == event.type) {
		if (SDL_BUTTON_RIGHT == event.button.button) {
			click = true;
		} 
        if (SDL_MOUSEBUTTONUP == event.type) {
if (SDL_BUTTON_LEFT == event.button.button) {
	}
}

}
}
