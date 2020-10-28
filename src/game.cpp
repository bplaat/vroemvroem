// VroemVroem - Game

#include "game.hpp"
#include <iostream>
#include "image.hpp"
#include "rect.hpp"
#include "fonts.hpp"
#include "terrain.hpp"
#include "nature.hpp"
#include "house.hpp"
#include "vehicle.hpp"

Game::Game(const char *title, int width, int height, bool fullscreen)
    : title(title), width(width), height(height)
{
    // Create window
    window = std::unique_ptr<SDL_Window, SDL_deleter>(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE));
    if (!window) {
        std::cerr << "[ERROR] Can't create the SDL window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowMinimumSize(window.get(), width / 2, height / 2);

    #if DEBUG
        SDL_MaximizeWindow(window.get());
    #endif

    setFullscreen(fullscreen);

    // Create canvas
    canvas = std::make_shared<Canvas>(window.get());

    // Load fonts
    Fonts::getInstance();

    // Load images
    Terrain::loadImages(canvas);
    Nature::loadImages(canvas);
    House::loadImages(canvas);
    Vehicle::loadImages(canvas);

    // Generate world
    world = std::make_shared<World>(1, 512, 512);

    camera = std::make_unique<Camera>(static_cast<float>(world->getWidth()) / 2, static_cast<float>(world->getHeight()) / 2,
        world->getWidth(), world->getHeight(), 2);
}

const char *Game::getTitle() const {
    return title;
}

int Game::getWidth() const {
    return width;
}

int Game::getHeight() const {
    return height;
}

bool Game::getFullscreen() const {
    return fullscreen;
}

void Game::setFullscreen(bool fullscreen) {
    this->fullscreen = fullscreen;
    if (fullscreen) {
        SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(window.get(), 0);
    }
}

void Game::handleEvent(const SDL_Event *event) {
    // Send all events to camera
    camera->handleEvent(event);

    // Handle key down events
    if (event->type == SDL_KEYUP) {
        if (event->key.keysym.sym == SDLK_F11) {
            setFullscreen(!fullscreen);
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
        stop();
    }
}

void Game::update(float delta) {
    // Update world
    world->update(delta);
}

void Game::draw() const {
    SDL_Renderer *renderer = canvas->getRenderer();

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
    SDL_RenderClear(renderer);

    // Draw world with camera
    world->draw(canvas.get(), camera.get());

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    // Draw debug label
    char debugLabel[128];
    sprintf(debugLabel, "camera.x = %.02f, camera.y = %.02f, camera.tileSize = %d", camera->getX(), camera->getY(), tileSize);

    std::unique_ptr<Image> debugLabelImage = Fonts::getInstance()->getTextFont()->render(canvas, debugLabel, 32, RGB(255, 255, 255));
    Rect debugLabelRect = { 16, 16, debugLabelImage->getWidth(), debugLabelImage->getHeight() };
    debugLabelImage->draw(&debugLabelRect);

    // Update screen
    SDL_RenderPresent(renderer);
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
        float delta = static_cast<double>((time - oldTime) * 1000) / SDL_GetPerformanceFrequency();

        // Update game
        update(delta);

        // Draw game
        draw();

        // Update old time
        oldTime = time;
    }
}

void Game::stop() {
    running = false;
}
