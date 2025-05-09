#include "Mouse.hpp"

#include "SDL_events.h"

void Mouse::XY(SDL_Event &event) {
	if (SDL_MOUSEMOTION == event.type) {
		SDL_GetMouseState(&x, &y);
		xpos = x;
		ypos = y;
	}
}
void Mouse::Clicked(SDL_Event &event) {
	if (SDL_MOUSEBUTTONDOWN == event.type) {
		if (SDL_BUTTON_LEFT == event.button.button) {
			click = true;
		}
	}
	if (SDL_MOUSEBUTTONUP == event.type) {
		if (SDL_BUTTON_LEFT == event.button.button) {
			click = false;
		}
	}
}
