
#include "Game.h"
#include "clock.hpp"

void Player::Update() {
	oldX = posx;
	oldY = posy;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	moving_left = state[SDL_SCANCODE_A];
	moving_up = state[SDL_SCANCODE_W];
	moving_down = state[SDL_SCANCODE_S];
	moving_right = state[SDL_SCANCODE_D];
	glm::vec2 move;
	oldX = posx;
	oldY = posy;
	move.x = 0;
	move.y = 0;
	if (moving_up) move.y -= 1 * Clock::delta * 0.5;
	if (moving_down) move.y += 1 * Clock::delta * 0.5;
	if (moving_right) {
		move.x += 1 * Clock::delta * 0.5;
		isFlipped = true;
	}
	if (moving_left) {
		move.x -= 1 * Clock::delta * 0.5;
		isFlipped = false;
	}
	if (moving_up && moving_right || moving_up && moving_left ||
		moving_down && moving_right || moving_down && moving_left) {
		move.x /= 1.414;
		move.y /= 1.414;
	}
	posx += move.x;
	posy += move.y;
	dest.x = posx - Game::Camera.x;
	dest.y = posy - Game::Camera.y;
	deltaX = posx - oldX;
	deltaY = posy - oldY;
	if (posx < 0) {
		posx = 0;
	}
	if (posy < 0) {
		posy = 0;
	}
	if (posx > 120 * 32 - dest.w) {
		posx = 120 * 32 - dest.w;
	}
	if (posy > 72 * 32 - dest.h) {
		posy = 72 * 32 - dest.h;
	}
}
void Player::Render() {
	if (!isFlipped)
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &dest, 0, NULL,
						 SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &dest);
}
