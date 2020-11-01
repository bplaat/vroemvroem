// VroemVroem - Image

#include "image.hpp"
#include <iostream>
#include "stb_image.h"

Image::Image(std::shared_ptr<Canvas> canvas, const char *path, bool transparent)
    : canvas(canvas), transparent(transparent)
{
    int channels;
    std::unique_ptr<uint8_t[], stbi_deleter> bitmap = std::unique_ptr<uint8_t[], stbi_deleter>(stbi_load(path, &width, &height, &channels, transparent ? STBI_rgb_alpha : STBI_rgb));
    if (!bitmap) {
        std::cerr << "[ERROR] Can't load image: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    loadBitmap(bitmap.get());
}

Image::Image(std::shared_ptr<Canvas> canvas, int width, int height, bool transparent, const uint8_t *bitmap)
    : canvas(canvas), width(width), height(height), transparent(transparent)
{
    loadBitmap(bitmap);
}

std::shared_ptr<Canvas> Image::getCanvas() const {
    return canvas;
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

bool Image::isTransparent() const {
    return transparent;
}

SDL_Texture *Image::getTexture() const {
    return texture.get();
}

void Image::draw(const Rect *rect) const {
    canvas->drawTexture(texture.get(), rect);
}

void Image::draw(const Rect *rect, float angle) const {
    canvas->drawTexture(texture.get(), rect, angle);
}

void Image::draw(const Rect *destinationRect, const Rect *sourceRect) const {
    canvas->drawTexture(texture.get(), destinationRect, sourceRect);
}

void Image::draw(const Rect *destinationRect, const Rect *sourceRect, float angle) const {
    canvas->drawTexture(texture.get(), destinationRect, sourceRect, angle);
}

void Image::loadBitmap(const uint8_t *bitmap) {
    std::unique_ptr<SDL_Surface, SDL_deleter> surface = std::unique_ptr<SDL_Surface, SDL_deleter>(SDL_CreateRGBSurfaceFrom(
        (void *)bitmap, width, height,
        transparent ? 32 : 24, transparent ? 4 * width : 3 * width,
        0x000000ff, 0x0000ff00, 0x00ff0000, transparent ? 0xff000000 : 0x00000000
    ));
    if (!surface) {
        std::cerr << "[ERROR] Can't create SDL surface: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    texture = std::unique_ptr<SDL_Texture, SDL_deleter>(SDL_CreateTextureFromSurface(canvas->getRenderer(), surface.get()));
    if (!texture) {
        std::cerr << "[ERROR] Can't create SDL texture: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}
