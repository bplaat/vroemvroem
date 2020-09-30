// VroemVroem - Game Object

#include "game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include "resources.hpp"
#include "image.hpp"

Game::Game() {
    // Create window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "[ERROR] Can't create the SDL window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_SetWindowMinimumSize(window, minWidth, minHeight);

    #if DEBUG
        SDL_MaximizeWindow(window);
    #endif

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "[ERROR] Can't create the SDL renderer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Load resources into singleton
    Resources *resources = Resources::getInstance();

    // Load terrain images
    /*  0 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/water_deep.png", false));
    /*  1 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/water.png", false));
    /*  2 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/sand1.png", false));
    /*  3 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/sand2.png", false));
    /*  4 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/grass1.png", false));
    /*  5 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/grass2.png", false));
    /*  6 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/dirt1.png", false));
    /*  7 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/dirt2.png", false));
    /*  8 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/stone1.png", false));
    /*  9 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/stone1.png", false));
    /* 10 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/snow1.png", false));
    /* 11 */ resources->terrainImages.push_back(new Image(renderer, "assets/images/terrain/snow2.png", false));

    // Load object images
    /*  0 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/bush1.png", true));
    /*  1 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/bush2.png", true));
    /*  2 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/tree1.png", true));
    /*  3 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/tree2.png", true));
    /*  4 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/tree3.png", true));
    /*  5 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/tree4.png", true));
    /*  6 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/trunk1.png", true));
    /*  7 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/trunk2.png", true));
    /*  8 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/rock1.png", true));
    /*  9 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/rock2.png", true));
    /* 10 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/house1.png", true));
    /* 11 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/house2.png", true));
    /* 12 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/house3.png", true));
    /* 13 */ resources->objectImages.push_back(new Image(renderer, "assets/images/objects/house4.png", true));

    // Generate world
    int seed = 1;
    int width = 512;
    int height = 512;
    world = new World(width, height, seed);
    camera = new Camera(world, (float)width / 2, (float)height / 2, 2);
}

Game::~Game() {
    delete Resources::getInstance();

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
