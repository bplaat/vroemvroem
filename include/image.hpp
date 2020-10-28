// VroemVroem - Image Header

#pragma once

#include <memory>
#include "canvas.hpp"
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "rect.hpp"

class Image {
    private:
        std::shared_ptr<Canvas> canvas;

        int width;

        int height;

        bool transparent;

        std::unique_ptr<SDL_Texture, SDL_deleter> texture;

    public:
        Image(std::shared_ptr<Canvas> canvas, const char *path, bool transparent);

        Image(std::shared_ptr<Canvas> canvas, int width, int height, bool transparent, const uint8_t *bitmap);

        std::shared_ptr<Canvas> getCanvas() const;

        int getWidth() const;

        int getHeight() const;

        bool isTransparent() const;

        SDL_Texture *getTexture() const;

        void draw(const Rect *rect) const;

        void draw(const Rect *rect, float angle) const;

    private:
        void loadBitmap(const uint8_t *bitmap);
};
