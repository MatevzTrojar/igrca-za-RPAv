

#include <iostream>

#include "GameObject.h"
#include "SDL_keyboard.h"
#include "SDL_platform.h"
#include "SDL_scancode.h"
int main() {
	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("GAME",	// creates a window
									   SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

	// triggers the program that controls
	// your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	// creates a renderer to render our images
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
	GameObject player;
	// creates a surface to load an image into the main memory

	// please provide a path for your image
	player.surface = IMG_Load("assets/textures/Test2.png");

	// loads image to our graphics hardware memory.
	player.tex = SDL_CreateTextureFromSurface(rend, player.surface);

	// clears main-memory
	SDL_FreeSurface(player.surface);

	// let us control our image position
	// so that we can move it with our keyboard.

	// connects our texture with dest to control position
	SDL_QueryTexture(player.tex, NULL, NULL, &player.dest.w, &player.dest.h);

	// adjust height and width of our image box.
	player.dest.w /= 3.5;
	player.dest.h /= 3.5;

	// sets initial x-position of object
	player.dest.x = (1000 - player.dest.w) / 2;

	// sets initial y-position of object
	player.dest.y = (1000 - player.dest.h) / 2;

	// controls animation loop
	int close = 0;

	// animation loop
	while (!close) {
		const Uint8* state = SDL_GetKeyboardState(NULL);
		player.moving_left = state[SDL_SCANCODE_A];
		player.moving_up = state[SDL_SCANCODE_W];
		player.moving_down = state[SDL_SCANCODE_S];
		player.moving_right = state[SDL_SCANCODE_D];
		SDL_Event event;

		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					close = 1;
					break;
			}
		}
		if (player.moving_down) player.dest.y += 10;
		if (player.moving_right) player.dest.x += 10;
		if (player.moving_up) player.dest.y -= 10;
		if (player.moving_left) player.dest.x -= 10;
		// right boundary
		if (player.dest.x + player.dest.w > 1000)
			player.dest.x = 1000 - player.dest.w;

		// left boundary
		if (player.dest.x < 0) player.dest.x = 0;

		// bottom boundary
		if (player.dest.y + player.dest.h > 1000)
			player.dest.y = 1000 - player.dest.h;

		// upper boundary
		if (player.dest.y < 0) player.dest.y = 0;

		// clears the screen
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, player.tex, NULL, &player.dest);

		// triggers the double buffers
		// for multiple rendering
		SDL_RenderPresent(rend);
		SDL_Delay(1000 / 60);
	}

	// destroy texture
	SDL_DestroyTexture(player.tex);

	// destroy renderer
	SDL_DestroyRenderer(rend);

	// destroy window
	SDL_DestroyWindow(win);

	// close SDL
	SDL_Quit();

	return 0;
}
