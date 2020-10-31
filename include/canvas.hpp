// VroemVroem - Canvas Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "rect.hpp"
#include "color.hpp"

class Canvas {
    private:
        std::unique_ptr<SDL_Renderer, SDL_deleter> renderer;

    public:
        Canvas(SDL_Window *window);

        SDL_Renderer *getRenderer() const;

        std::unique_ptr<Rect> getRect() const;

        void clear(const Color *color);

        void fillRect(const Rect *rect, const Color *color);

        void drawTexture(SDL_Texture *texture, const Rect *rect);

        void drawTexture(SDL_Texture *texture, const Rect *rect, float angle);

        void present();

};
