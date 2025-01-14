#include <SDL.h>
#include <SDL_events.h>
using namespace std;
#include <iostream>
#include <SDL_render.h>
#include <cstdlib>

int main(int argumentCount, char * arguments[])
{
  SDL_Window * window = SDL_CreateWindow("Igrica", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 700, SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);
    SDL_SetRenderDrawColor(renderer , 255 , 0,0,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    
    while(true)
    {
        if(SDL_PollEvent(&event)){
            if(SDL_QUIT == event.type){
                break;
            }
            if(SDL_KEYDOWN== event.type){
                //cout<<"key is down"<<endl;
                if(SDLK_a == event.key.keysym.sym)
                    cout<<"a is down - move left"<<endl;//tuki je WASD detection
                else  if(SDLK_s == event.key.keysym.sym)
                    cout<<"s is down - move back"<<endl;
                else if(SDLK_w == event.key.keysym.sym)
                    cout<<"w is down - move up"<<endl;
                else  if(SDLK_d == event.key.keysym.sym)
                    cout<<"d down - move right"<<endl;

            }
            else if(SDL_KEYUP== event.type){
              //  cout<<"key is up"<<endl;
            }
        }
    }
      return EXIT_SUCCESS;
}
