// VroemVroem - Image Object

#include "image.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include "stb_image.h"

Image::Image(SDL_Renderer *renderer, const char *path, bool transparent)
    : transparent(transparent)
{
    int channels;
    uint8_t *data = stbi_load(path, &width, &height, &channels, transparent ? STBI_rgb_alpha : STBI_rgb);
    if (data == nullptr) {
        std::cerr << "[ERROR] Can't load image: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    int depth, pitch;
    if (transparent) {
        depth = 32;
        pitch = 4 * width;
    } else {
        depth = 24;
        pitch = 3 * width;
    }

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)data, width, height, depth, pitch,
        0x000000ff, 0x0000ff00, 0x00ff0000, transparent ? 0xff000000 : 0);

    if (surface == nullptr) {
        std::cerr << "[ERROR] Can't create SDL surface: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cerr << "[ERROR] Can't create SDL texture: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(surface);

    stbi_image_free(data);
}

Image::~Image() {
    SDL_DestroyTexture(texture);
}

void Image::draw(SDL_Renderer *renderer, SDL_Rect *destinationRect, SDL_Rect *sourceRect) {
    SDL_RenderCopy(renderer, texture, sourceRect, destinationRect);
}
