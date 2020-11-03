// VroemVroem - Game

#include "game.hpp"
#include "config.hpp"
#include <iostream>
#include "image.hpp"
#include "rect.hpp"
#include "fonts.hpp"
#include "objects/terrain.hpp"
#include "objects/nature.hpp"
#include "objects/house.hpp"
#include "objects/road.hpp"
#include "objects/vehicle.hpp"
#ifdef DEBUG
#include "pages/game_page.hpp"
#else
#include "pages/intro_page.hpp"
#endif

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
    #if DEBUG
    SDL_MaximizeWindow(window.get());
    #endif

    // Set window fullscreen
    setFullscreen(fullscreen);

    // Create canvas
    canvas = std::make_shared<Canvas>(window.get());

    // Set window icon by loading it as surface
    const char *iconPath = "assets/images/icon.png";
    int iconWidth, iconHeight, iconChannels;
    bool iconTransparent = false;

    std::unique_ptr<uint8_t[], stbi_deleter> iconBitmap = std::unique_ptr<uint8_t[], stbi_deleter>(stbi_load(iconPath, &iconWidth, &iconHeight, &iconChannels, iconTransparent ? STBI_rgb_alpha : STBI_rgb));
    if (!iconBitmap) {
        std::cerr << "[ERROR] Can't load image: " << iconPath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::unique_ptr<SDL_Surface, SDL_deleter> iconSurface = std::unique_ptr<SDL_Surface, SDL_deleter>(SDL_CreateRGBSurfaceFrom(
        (void *)iconBitmap.get(), iconWidth, iconHeight,
        iconTransparent ? 32 : 24, iconTransparent ? 4 * iconWidth : 3 * iconWidth,
        0x000000ff, 0x0000ff00, 0x00ff0000, iconTransparent ? 0xff000000 : 0x00000000
    ));
    if (!iconSurface) {
        std::cerr << "[ERROR] Can't create SDL surface: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowIcon(window.get(), iconSurface.get());
}

Game *Game::getInstance() {
    if (!instance) {
        instance = std::make_unique<Game>(Config::name, 1280, 720, false);
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

bool Game::handleEvent(const SDL_Event *event) {
    // Handle key down events
    if (event->type == SDL_KEYUP) {
        if (event->key.keysym.sym == SDLK_F11) {
            setFullscreen(!fullscreen);
            return true;
        }
    }

    // Handle window resize events
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        width = event->window.data1;
        height = event->window.data2;
    }

    // Set event to current page
    if (page->handleEvent(event)) {
        return true;
    }

    // Handle window close events
    if (event->type == SDL_QUIT) {
        stop();
        return true;
    }

    return false;
}

void Game::start() {
    // Load fonts
    Fonts::getInstance();

    // Load images
    Objects::Terrain::loadImages(canvas);
    Objects::Nature::loadImages(canvas);
    Objects::House::loadImages(canvas);
    Objects::Road::loadImage(canvas);
    Objects::Vehicle::loadImages(canvas);

    #ifdef DEBUG
    // Create game page
    page = std::make_unique<Pages::GamePage>();
    #else
    // Create intro page
    page = std::make_unique<Pages::IntroPage>();
    #endif

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
        float delta = (double)((time - oldTime) * 1000) / SDL_GetPerformanceFrequency();

        // Update current page
        page->update(delta);

        // Draw current page
        page->draw(canvas);
        canvas->present();

        // Update old time
        oldTime = time;
    }
}

void Game::stop() {
    running = false;
}
