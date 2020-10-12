// VroemVroem - Utils Header

#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <cstdint>
#include "stb_image.h"
#include "stb_truetype.h"

#define RGB(r, g, b) (uint32_t)((uint8_t)r | ((uint8_t)g << 8) | ((uint8_t)b << 16))

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define degreesToRadians(degrees) (degrees * M_PI / 180.0)

#define radiansToDegrees(radians) (radians * 180.0 / M_PI)

struct Rect {
    int x;
    int y;
    int width;
    int height;
};

struct SDL_deleter {
    void operator()(SDL_Window *window) const { SDL_DestroyWindow(window); }
    void operator()(SDL_Renderer *renderer) const { SDL_DestroyRenderer(renderer); }
    void operator()(SDL_Surface *surface) const { SDL_FreeSurface(surface); }
    void operator()(SDL_Texture *texture) const { SDL_DestroyTexture(texture); }
};

struct stbi_deleter {
    void operator()(uint8_t *image) const { stbi_image_free(image); }
};

struct stbtt_deleter {
    void operator()(uint8_t *bitmap) const { stbtt_FreeBitmap(bitmap, nullptr); }
};
