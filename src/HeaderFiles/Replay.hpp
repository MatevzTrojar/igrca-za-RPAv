#pragma once
#include <SDL.h>

#include <vector>

#include "Game.h"

class Replay {
   public:
	static void initReplay(Game* game);
	static void Update(Game* game);
	static void render(Game* game);

   private:
	static std::vector<Game::DeltaPlayer> replayPath;
	static size_t currentFrame;
};
