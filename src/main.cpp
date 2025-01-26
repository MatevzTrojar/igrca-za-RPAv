#include <ctime>
#include "Game.h"
#include "SDL_joystick.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"

Clock* ura= new Clock;
Game* game = nullptr;

int main(int argc, char* argv[]) {
//	const int FPS = 60;
//	Uint32 frameStart;
    
	game = new Game();
	game->init("zivali", 1920, 1080, false);
	while (game->running()) {
	//	frameStart = SDL_GetTicks();
        ura -> tick();
		game->handleEvents();
		game->update(ura);
		game->render();
	}
/*	if (1000 / FPS > SDL_GetTicks() - frameStart) {
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - frameStart));
	}*/
	game->clean();

	return 0;
}
