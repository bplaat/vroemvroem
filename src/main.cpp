// VroemVroem - A highway simulator school project
// Made by Bastiaan van der Plaat (https://bastiaan.ml/)

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "game.hpp"

int main() {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    Game game;
    game.start();

    SDL_Quit();
    return EXIT_SUCCESS;
}
