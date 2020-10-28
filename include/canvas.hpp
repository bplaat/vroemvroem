// VroemVroem - Canvas Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "rect.hpp"

class Canvas {
    private:
        std::unique_ptr<SDL_Renderer, SDL_deleter> renderer;

    public:
        Canvas(SDL_Window *window);

        SDL_Renderer *getRenderer() const;

        std::unique_ptr<Rect> getRect() const;
};
