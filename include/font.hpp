// VroemVroem - Font Header

#pragma once

#include <memory>
#include "stb_truetype.h"
#include "canvas.hpp"
#include "image.hpp"

class Font {
    private:
        std::unique_ptr<uint8_t[]> fontBuffer;

        stbtt_fontinfo fontInfo;

    public:
        Font(const char *path);

        int measure(const char *text, int textSize);

        std::unique_ptr<Image> render(std::shared_ptr<Canvas> canvas, const char *text, int textSize, uint32_t textColor);
};
