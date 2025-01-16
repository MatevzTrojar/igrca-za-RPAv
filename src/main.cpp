#include <SDL2/SDL.h>
#include <algorithm>
#include <ctime>
#include <glm/vector_relational.hpp>
#include <SDL2/SDL_timer.h>
#include <SDL_image.h>
#include <glm/vec2.hpp>
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"
#include <glm/glm.hpp>
#include "clock.hpp"
#include<iostream>
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
    
    Clock clock;

	// creates a renderer to render our images
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	// creates a surface to load an image into the main memory
	SDL_Surface* surface;

	// please provide a path for your image
	surface = IMG_Load("assets/textures/test.jpg");

	// loads image to our graphics hardware memory.
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);

	// clears main-memory
	SDL_FreeSurface(surface);

	// let us control our image position
	// so that we can move it with our keyboard.
	SDL_Rect dest;

	// connects our texture with dest to control position
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	// adjust height and width of our image box.
	dest.w /= 3.5;
	dest.h /= 3.5;

	// sets initial x-position of object
	dest.x = (1000 - dest.w) / 2;

	// sets initial y-position of object
	dest.y = (1000 - dest.h) / 2;

	// controls animation loop
	int close = 0;

	// speed of box
	int speed = 300;

	// animation loop
	while (!close) {
        clock.tick();
		SDL_Event event;
        glm::vec2 move;
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					close = 1;
					break;

				case SDL_KEYDOWN:
					// keyboard API for key pressed
                   		if(SDLK_a == event.key.keysym.sym){
                            move.x-=1;
                        }
					    if(SDLK_w == event.key.keysym.sym){
                            move.y-=1;
                        }
					    if(SDLK_s == event.key.keysym.sym){
                            move.y+=1;
                        }
					    if(SDLK_d == event.key.keysym.sym){
                            move.x+=1;
                        }
                        break;
				case SDL_KEYUP:
					// keyboard API for key pressed
                   		if(SDLK_a == event.key.keysym.sym){
                            move.x+=1;
                        }
					    if(SDLK_w == event.key.keysym.sym){
                            move.y+=1;
                        }
					    if(SDLK_s == event.key.keysym.sym){
                            move.y-=1;
                        }
					    if(SDLK_d == event.key.keysym.sym){
                            move.x-=1;
                        }
                        break;

			}
		}
        glm::vec2 finalMove = glm::normalize(move);
        finalMove *= clock.delta;
        dest.x += finalMove.x;
        dest.y += finalMove.y;
        std::cout<<clock.delta<<std::endl;
		// right boundary
		if (dest.x + dest.w > 1000) dest.x = 1000 - dest.w;

		// left boundary
		if (dest.x < 0) dest.x = 0;

		// bottom boundary
		if (dest.y + dest.h > 1000) dest.y = 1000 - dest.h;

		// upper boundary
		if (dest.y < 0) dest.y = 0;

		// clears the screen
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, tex, NULL, &dest);

		// triggers the double buffers
		// for multiple rendering
		SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
	}
    
	// destroy texture
	SDL_DestroyTexture(tex);

	// destroy renderer
	SDL_DestroyRenderer(rend);

	// destroy window
	SDL_DestroyWindow(win);

	// close SDL
	SDL_Quit();

	return 0;
}
