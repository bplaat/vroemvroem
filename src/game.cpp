// VroemVroem - Game Object

#include "game.hpp"
#include <SDL2/SDL.h>

Game::Game() {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    SDL_SetWindowMinimumSize(window, minWidth, minHeight);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Game init code
}

void Game::handleEvent(SDL_Event *event) {
    if (event->type == SDL_KEYUP) {
        if (event->key.keysym.sym == SDLK_F11) {
            if (!fullscreen) {
                fullscreen = true;
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            } else {
                fullscreen = false;
                SDL_SetWindowFullscreen(window, 0);
            }
        }
    }

    if (event->type == SDL_WINDOWEVENT) {
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            width = event->window.data1;
            height = event->window.data2;
        }
    }

    if (event->type == SDL_QUIT) {
        running = false;
    }
}

void Game::update(double delta) {
    (void)delta;

    // Game update code
}

void Game::draw() {
    SDL_RenderClear(renderer);

    // Game drawing code

    SDL_RenderPresent(renderer);
}

void Game::start() {
    // Game loop
    uint64_t oldTime = SDL_GetPerformanceCounter();
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        time = SDL_GetPerformanceCounter();
        double delta = ((time - oldTime) * 1000) / (double)SDL_GetPerformanceFrequency();
        update(delta);

        draw();

        oldTime = time;
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
