// VroemVroem - A highway simulator school project
// Made by Bastiaan van der Plaat (https://bastiaan.ml/)

#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[ERROR] Can't init SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    Game game;
    game.start();

    SDL_Quit();
    return EXIT_SUCCESS;
}
