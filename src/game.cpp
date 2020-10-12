// VroemVroem - Game Object

#include "game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "resources.hpp"
#include "image.hpp"

Game::Game() {
    // Create window
    window = std::unique_ptr<SDL_Window, SDL_deleter>(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE));
    if (!window) {
        std::cerr << "[ERROR] Can't create the SDL window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowMinimumSize(window.get(), minWidth, minHeight);

    #if DEBUG
        SDL_MaximizeWindow(window.get());
    #endif

    // Create renderer
    renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);
    if (!renderer) {
        std::cerr << "[ERROR] Can't create the SDL renderer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Load resources
    resources = std::make_shared<Resources>(renderer);

    // Generate world
    int seed = 1;
    int width = 512;
    int height = 512;

    world = std::make_shared<World>(renderer, resources, width, height, seed);

    camera = std::make_unique<Camera>(world, (float)width / 2, (float)height / 2, 2);
}

void Game::handleEvent(const SDL_Event *event) {
    // Send all events to camera
    camera->handleEvent(event);

    // Handle key down events
    if (event->type == SDL_KEYUP) {
        if (event->key.keysym.sym == SDLK_F11) {
            if (!fullscreen) {
                fullscreen = true;
                SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
            } else {
                fullscreen = false;
                SDL_SetWindowFullscreen(window.get(), 0);
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
    // Update world
    world->update(delta);
}

void Game::draw() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer.get(), 17, 17, 17, 255);
    SDL_RenderClear(renderer.get());

    // Draw world with camera
    world->draw(camera.get());

    // Draw debug label
    char debugLabel[128];
    sprintf(debugLabel, "camera.x = %.02f, camera.y = %.02f, camera.tileSize = %d", camera->x, camera->y, camera->tileSize);

    auto debugLabelImage = resources->textFont->render(renderer, debugLabel, 32, RGB(255, 255, 255));
    Rect debugLabelRect = { 16, 16, debugLabelImage->width, debugLabelImage->height };
    debugLabelImage->draw(&debugLabelRect);

    // Update screen
    SDL_RenderPresent(renderer.get());
}

void Game::start() {
    // Old time variable
    uint64_t oldTime = SDL_GetPerformanceCounter();

    // Game loop
    running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        // Calculate new delta via old and new time
        time = SDL_GetPerformanceCounter();
        float delta = ((time - oldTime) * 1000) / (float)SDL_GetPerformanceFrequency();

        // Update game
        update(delta);

        // Draw game
        draw();

        // Update old time
        oldTime = time;
    }
}
