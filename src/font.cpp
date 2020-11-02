// VroemVroem - Font

#include "font.hpp"
#include <iostream>
#include <cstring>
#include <memory>
#include "canvas.hpp"
#include "utils.hpp"

Font::Font(const char *path) {
    FILE *fontFile = fopen(path, "rb");
    if (!fontFile) {
        std::cerr << "[ERROR] Can't read font file: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    fseek(fontFile, 0, SEEK_END);
    size_t fontFileSize = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    fontBuffer = std::make_unique<uint8_t[]>(fontFileSize + 1);
    fread(fontBuffer.get(), 1, fontFileSize, fontFile);
    fontBuffer[fontFileSize] = 0;
    fclose(fontFile);

    if (stbtt_InitFont(&fontInfo, fontBuffer.get(), stbtt_GetFontOffsetForIndex(fontBuffer.get(), 0)) == 0) {
        std::cerr << "[ERROR] Can't load font: " << path << std::endl;
        exit(EXIT_FAILURE);
    }
}

int Font::measure(const char *text, int textSize) {
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, textSize);
    int xpadding = textSize / 4;
    int width = xpadding * 2;
    for (size_t i = 0; i < strlen(text); i++) {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&fontInfo, text[i], &advance, &lsb);

        width += advance * scale;

        if (text[i + 1]) {
            width += stbtt_GetCodepointKernAdvance(&fontInfo, text[i], text[i + 1]) * scale;
        }
    }
    return width;
}

// BUG
std::unique_ptr<Image> Font::render(std::shared_ptr<Canvas> canvas, const char *text, int textSize, const Color *textColor) {
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, textSize);
    int xpadding = textSize / 4;

    int width = measure(text, textSize);
    int height = textSize;

    std::unique_ptr<uint8_t[]> bitmap = std::make_unique<uint8_t[]>(height * width);
    for (int i = 0; i < height * width; i++) {
        bitmap[i] = 0;
    }

    int x = xpadding;

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);

    for (size_t i = 0; i < strlen(text); i++) {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&fontInfo, text[i], &advance, &lsb);

        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&fontInfo, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        int y = ascent + c_y1;

        int characterWidth, characterHeight;
        std::unique_ptr<uint8_t[], stbtt_deleter> characterBitmap = std::unique_ptr<uint8_t[], stbtt_deleter>(stbtt_GetCodepointBitmap(&fontInfo, 0, scale, text[i], &characterWidth, &characterHeight, 0, 0));
        for (int cy = 0; cy < characterHeight; cy++) {
            for (int cx = 0; cx < characterWidth; cx++) {
                int pos = (y + cy) * width + (x + cx);
                bitmap[pos] = std::max(bitmap[pos], characterBitmap[cy * characterWidth + cx]);
            }
        }

        x += advance * scale;

        if (text[i + 1]) {
            x += stbtt_GetCodepointKernAdvance(&fontInfo, text[i], text[i + 1]) * scale;
        }
    }

    std::unique_ptr<uint8_t[]> coloredBitmap = std::make_unique<uint8_t[]>(height * width * 4);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pos = (y * width + x) * 4;
            coloredBitmap[pos] = textColor->red;
            coloredBitmap[pos + 1] = textColor->green;
            coloredBitmap[pos + 2] = textColor->blue;
            coloredBitmap[pos + 3] = bitmap[y * width + x];
        }
    }

    return std::make_unique<Image>(canvas, width, height, true, coloredBitmap.get());
}
