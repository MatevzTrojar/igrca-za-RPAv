#include <SDL2/SDL.h>
struct Clock {
	unsigned int last_tick_time = 0;
	unsigned int delta = 0;

	void tick() {
        unsigned int tick_time = SDL_GetTicks();
		delta = tick_time - last_tick_time;
		last_tick_time = tick_time;
	}
};
