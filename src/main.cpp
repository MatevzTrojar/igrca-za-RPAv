#include "Game.h"
#include "SDL_joystick.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"

Game* game = nullptr;
int main(int argc, char* argv[]) {
	const int FPS = 60;
	Uint32 frameStart;

	game = new Game();
	game->init("zivali", 1000, 1000, false);

	while (game->running()) {
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();
	}
	if (1000 / FPS > SDL_GetTicks() - frameStart) {
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - frameStart));
	}
	game->clean();

	return 0;
}
