#include "SDL_events.h"
#include "SDL_haptic.h"
class Mouse{
    int x,y;
    bool click = false;
    public:
    void Update(SDL_Event);
};
