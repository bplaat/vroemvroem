// VroemVroem - Timer

#include "timer.hpp"
#include <SDL2/SDL.h>

uint32_t Timer::callback(uint32_t interval, void *param) {
    SDL_UserEvent userevent;
    userevent.type = SDL_USEREVENT;
    userevent.code = (intptr_t)param;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);

    return interval;
}
