// VroemVroem - Game

#include "game.hpp"
#include <iostream>
#include "image.hpp"
#include "rect.hpp"
#include "fonts.hpp"
#include "objects/terrain.hpp"
#include "objects/nature.hpp"
#include "objects/house.hpp"
#include "objects/vehicle.hpp"
#include "pages/menu.hpp"

std::unique_ptr<Game> Game::instance = nullptr;

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

    // Make window maximezed when debugging
    // #if DEBUG
    //     SDL_MaximizeWindow(window.get());
    // #endif

    // Set window fullscreen
    setFullscreen(fullscreen);

    // Create canvas
    canvas = std::make_shared<Canvas>(window.get());

    // Set window icon
    // std::unique_ptr<Image> iconImage = std::make_unique<Image>(canvas, "assets/images/icon.png", false);
    // SDL_SetWindowIcon(window.get(), iconImage->getSurface());
}

Game *Game::getInstance() {
    if (!instance) {
        instance = std::make_unique<Game>("VroemVroem", 1280, 720, false);
    }
    return instance.get();
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

std::shared_ptr<Canvas> Game::getCanvas() const {
    return canvas;
}

Pages::Page *Game::getPage() const {
    return page.get();
}

void Game::setPage(std::unique_ptr<Pages::Page> page) {
    this->page = std::move(page);
}

void Game::handleEvent(const SDL_Event *event) {
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

    // Set event to current page
    page->handleEvent(event);

    // Handle window close events
    if (event->type == SDL_QUIT) {
        stop();
    }
}

void Game::start() {
    // Load fonts
    Fonts::getInstance();

    // Load images
    Objects::Terrain::loadImages(canvas);
    Objects::Nature::loadImages(canvas);
    Objects::House::loadImages(canvas);
    Objects::Vehicle::loadImages(canvas);

    // Create menu page
    page = std::make_unique<Pages::MenuPage>();

    // Old time variable
    uint64_t oldTime = SDL_GetPerformanceCounter();

    // Game loop
    running = true;
    while (running) {
        // Handle window events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        // Calculate new delta via old and new time
        time = SDL_GetPerformanceCounter();
        float delta = static_cast<double>((time - oldTime) * 1000) / SDL_GetPerformanceFrequency();

        // Update current page
        page->update(delta);

        // Draw current page
        page->draw(canvas.get());
        SDL_RenderPresent(canvas->getRenderer());

        // Update old time
        oldTime = time;
    }
}

void Game::stop() {
    running = false;
}
