// VroemVroem - Image Object Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "utils.hpp"

class Image {
    public:
        std::shared_ptr<SDL_Renderer> renderer;
        int width;
        int height;
        bool transparent;
        std::unique_ptr<SDL_Texture, SDL_deleter> texture;

        Image(std::shared_ptr<SDL_Renderer> renderer, const char *path, bool transparent);

        Image(std::shared_ptr<SDL_Renderer> renderer, int width, int height, bool transparent, const uint8_t *bitmap);

        void draw(const Rect *destinationRect) const;

        void draw(const Rect *destinationRect, float angle) const;

        void draw(const Rect *destinationRect, const Rect *sourceRect) const;

        void draw(const Rect *destinationRect, const Rect *sourceRect, float angle) const;

    private:
        void loadBitmap(const uint8_t *bitmap);
};
