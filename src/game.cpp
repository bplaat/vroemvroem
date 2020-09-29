// VroemVroem - Game Object

#include "game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include "image.hpp"

Game::Game() {
    // Create window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "[ERROR] Can't create the SDL window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_SetWindowMinimumSize(window, minWidth, minHeight);

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "[ERROR] Can't create the SDL renderer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Load terrain images
    terrainImages[0] = new Image(renderer, "assets/images/terrain/water_deep.png", false);
    terrainImages[1] = new Image(renderer, "assets/images/terrain/water.png", false);
    terrainImages[2] = new Image(renderer, "assets/images/terrain/sand1.png", false);
    terrainImages[3] = new Image(renderer, "assets/images/terrain/sand2.png", false);
    terrainImages[4] = new Image(renderer, "assets/images/terrain/grass1.png", false);
    terrainImages[5] = new Image(renderer, "assets/images/terrain/grass2.png", false);
    terrainImages[6] = new Image(renderer, "assets/images/terrain/dirt1.png", false);
    terrainImages[7] = new Image(renderer, "assets/images/terrain/dirt2.png", false);
    terrainImages[8] = new Image(renderer, "assets/images/terrain/stone1.png", false);
    terrainImages[9] = new Image(renderer, "assets/images/terrain/stone1.png", false);
    terrainImages[10] = new Image(renderer, "assets/images/terrain/snow1.png", false);
    terrainImages[11] = new Image(renderer, "assets/images/terrain/snow2.png", false);

    // Generate world
    int width = 512;
    int height = 512;
    world = new World(this, width, height, 0);
    camera = new Camera(width / 2, height / 2, 2);
}

Game::~Game() {
    for (int i = 0; i < (int)(sizeof(terrainImages) / sizeof(Image *)); i++) {
        delete terrainImages[i];
    }

    delete world;

    delete camera;

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
}

void Game::handleEvent(SDL_Event *event) {
    // Send all events to camera
    camera->handleEvent(event);

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

void Game::update(float delta) {
    world->update(delta);
}

void Game::draw() {
    SDL_RenderClear(renderer);

    world->draw(renderer, camera);

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
}
