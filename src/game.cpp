// VroemVroem - Game Object

#include "game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include "utils.hpp"

Game::Game() {
    // Create window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "[ERROR] Can't create the SDL window: " << SDL_GetError();
        exit(EXIT_FAILURE);
    }
    SDL_SetWindowMinimumSize(window, minWidth, minHeight);

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "[ERROR] Can't create the SDL renderer: " << SDL_GetError();
        exit(EXIT_FAILURE);
    }

    // Load terrain images
    terrainImages[0] = LoadImage(renderer, "assets/images/grass1.png", false);
    terrainImages[1] = LoadImage(renderer, "assets/images/grass2.png", false);
    terrainImages[2] = LoadImage(renderer, "assets/images/dirt1.png", false);
    terrainImages[3] = LoadImage(renderer, "assets/images/dirt2.png", false);
    terrainImages[4] = LoadImage(renderer, "assets/images/sand1.png", false);
    terrainImages[5] = LoadImage(renderer, "assets/images/sand2.png", false);
}

void Game::handleEvent(SDL_Event *event) {
    // Handle key down events
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

    // Handle window resize events
    if (event->type == SDL_WINDOWEVENT) {
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            width = event->window.data1;
            height = event->window.data2;
        }
    }

    // Handle window close events
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
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        // Calculate new delta
        time = SDL_GetPerformanceCounter();
        double delta = ((time - oldTime) * 1000) / (double)SDL_GetPerformanceFrequency();

        // Update game
        update(delta);

        // Draw game
        draw();

        // Update old time
        oldTime = time;
    }

    // When closing delete all game objects
    for (int i = 0; i < (int)(sizeof(terrainImages) / sizeof(SDL_Texture *)); i++) {
        SDL_DestroyTexture(terrainImages[i]);
    }

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
}
