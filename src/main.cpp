#include <ctime>

#include "Game.h"
Clock* ura = new Clock;
Game* game = nullptr;

int main(int argc, char* argv[]) {
	game = new Game();
	game->init("zivali", 1920, 1080, false);
	while (game->running()) {
		ura->tick();
		game->handleEvents();
		game->update(ura);
		game->render();
	}
	game->clean();
	return 0;
}
