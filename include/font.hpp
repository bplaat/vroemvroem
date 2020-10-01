// VroemVroem - Font Object Header

#pragma once

#include <cstdint>
#include "stb_truetype.h"
#include <SDL2/SDL.h>

class Font {
    public:
        uint8_t *buffer;
        stbtt_fontinfo fontInfo;

        Font(const char *path);

        ~Font();

        int measure(const char *text, int size);

        SDL_Texture *render(SDL_Renderer *renderer, const char *text, int size, uint32_t color);
};
