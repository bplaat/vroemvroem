// VroemVroem - Game Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "canvas.hpp"
#include "pages/page.hpp"

class Game {
    private:
        static std::unique_ptr<Game> instance;

        const char *title;

        int width;

        int height;

        bool fullscreen;

        uint64_t time = 0;

        bool running = false;

        std::unique_ptr<SDL_Window, SDL_deleter> window;

        std::shared_ptr<Canvas> canvas;

        std::unique_ptr<Pages::Page> page;

    public:
        Game(const char *title, int width, int height, bool fullscreen);

        static Game *getInstance();

        const char *getTitle() const;

        int getWidth() const;

        int getHeight() const;

        bool getFullscreen() const;

        void setFullscreen(bool fullscreen);

        std::shared_ptr<Canvas> getCanvas() const;

        Pages::Page *getPage() const;

        void setPage(std::unique_ptr<Pages::Page> page);

        void start();

        void stop();

    private:
        bool handleEvent(const SDL_Event *event);
};
