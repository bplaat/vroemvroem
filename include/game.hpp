// VroemVroem - Game Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "canvas.hpp"
#include "world.hpp"
#include "camera.hpp"

class Game {
    private:
        const char *title;
        int width;
        int height;
        bool fullscreen;
        uint64_t time = 0;
        bool running = false;

        std::unique_ptr<SDL_Window, SDL_deleter> window;
        std::shared_ptr<Canvas> canvas;
        std::shared_ptr<World> world;
        std::unique_ptr<Camera> camera;

    public:
        Game(const char *title, int width, int height, bool fullscreen);

        const char *getTitle() const;

        int getWidth() const;

        int getHeight() const;

        bool getFullscreen() const;

        void setFullscreen(bool fullscreen);

        void start();

        void stop();

    private:
        void handleEvent(const SDL_Event *event);

        void update(float delta);

        void draw() const;
};
