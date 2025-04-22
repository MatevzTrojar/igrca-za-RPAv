
#include "SDL_events.h"

class Mouse {
   public:
	int x, y;
	float xpos, ypos;
	bool click = false;
	void XY(SDL_Event &event);
	void Clicked(SDL_Event &event);
};
