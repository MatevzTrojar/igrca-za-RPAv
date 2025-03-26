#include <cstdio>
#include <ctime>
#include <exception>

#include "Game.h"
Clock* ura = new Clock;
Game* game = nullptr;

int main(int argc, char* argv[]) {
	game = new Game();
	game->init("zivali", 1920, 1088, false);
   ura->last_tick_time = SDL_GetTicks();
	while (game->running()) {
		ura->tick();
		game->handleEvents();
        if(!game->victory)
		game->update(ura);
		game->render();
	}
	game->clean();
	return 0;
}
