// VroemVroem - Font Object Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "stb_truetype.h"
#include "image.hpp"

class Font {
    public:
        std::unique_ptr<uint8_t[]> fontBuffer;
        stbtt_fontinfo fontInfo;

        Font(const char *path);

        int measure(const char *text, int textSize);

        std::unique_ptr<Image> render(std::shared_ptr<SDL_Renderer> renderer, const char *text, int textSize, uint32_t textColor);
};
