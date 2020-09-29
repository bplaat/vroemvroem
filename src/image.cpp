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

    uint32_t rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        int shift = transparent ? 0 : 8;
        rmask = 0xff000000 >> shift;
        gmask = 0x00ff0000 >> shift;
        bmask = 0x0000ff00 >> shift;
        amask = 0x000000ff >> shift;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = transparent ? 0xff000000 : 0;
    #endif

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)data, width, height, depth, pitch, rmask, gmask, bmask, amask);
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
