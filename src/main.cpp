#include <cstdio>
#include <ctime>
#include <exception>

#include "Game.h"
Game* game = nullptr;
unsigned int Clock::last_tick_time = 0;
unsigned int Clock::delta = 0;
int main(int argc, char* argv[]) {
	game = new Game();
	game->init("zivali", 1920, 1088, false);
	while (game->running()) {
        Clock::tick();
		game->handleEvents();
        if(!game->victory)
		game->update();
		game->render();
	}
	game->clean();
	return 0;
}
