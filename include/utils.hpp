// VroemVroem - Utils Header

#pragma once

#include <cmath>
#include <SDL2/SDL.h>
#include "stb_image.h"
#include "stb_truetype.h"

// ### Math ###

// Mirosoft C++ libs don't define PI :(
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define radians(degrees) (static_cast<double>(degrees) * M_PI / 180.0)

#define degrees(radians) (static_cast<double>(radians) * 180.0 / M_PI)


// ### Deleters ###

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
