// VroemVroem - Image Object

#include "image.hpp"
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include "stb_image.h"

Image::Image(std::shared_ptr<SDL_Renderer> renderer, const char *path, bool transparent)
    : renderer(renderer), transparent(transparent)
{
    int channels;
    auto bitmap = std::unique_ptr<uint8_t[], stbi_deleter>(stbi_load(path, &width, &height, &channels, transparent ? STBI_rgb_alpha : STBI_rgb));
    if (!bitmap) {
        std::cerr << "[ERROR] Can't load image: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    loadBitmap(bitmap.get());
}

Image::Image(std::shared_ptr<SDL_Renderer> renderer, int width, int height, bool transparent, const uint8_t *bitmap)
    : renderer(renderer), width(width), height(height), transparent(transparent)
{
    loadBitmap(bitmap);
}

void Image::draw(const Rect *destinationRect) const {
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, (SDL_Rect *)destinationRect);
}

void Image::draw(const Rect *destinationRect, float angle) const {
    SDL_RenderCopyEx(renderer.get(), texture.get(), nullptr, (SDL_Rect *)destinationRect, angle, nullptr, SDL_FLIP_NONE);
}

void Image::draw(const Rect *destinationRect, const Rect *sourceRect) const {
    SDL_RenderCopy(renderer.get(), texture.get(), (SDL_Rect *)sourceRect, (SDL_Rect *)destinationRect);
}

void Image::draw(const Rect *destinationRect, const Rect *sourceRect, float angle) const {
    SDL_RenderCopyEx(renderer.get(), texture.get(), (SDL_Rect *)sourceRect, (SDL_Rect *)destinationRect, angle, nullptr, SDL_FLIP_NONE);
}

void Image::loadBitmap(const uint8_t *bitmap) {
    auto surface = std::unique_ptr<SDL_Surface, SDL_deleter>(SDL_CreateRGBSurfaceFrom(
        (void *)bitmap, width, height,
        transparent ? 32 : 24, transparent ? 4 * width : 3 * width,
        0x000000ff, 0x0000ff00, 0x00ff0000, transparent ? 0xff000000 : 0x00000000
    ));
    if (!surface) {
        std::cerr << "[ERROR] Can't create SDL surface: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    texture = std::unique_ptr<SDL_Texture, SDL_deleter>(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));
    if (!texture) {
        std::cerr << "[ERROR] Can't create SDL texture: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}
