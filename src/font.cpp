// VroemVroem - Font Object

#include "font.hpp"
#include <iostream>
#include <cstring>
#include "stb_truetype.h"
#include <SDL2/SDL.h>

Font::Font(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == nullptr) {
        std::cerr << "[ERROR] Can't read font file: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = new uint8_t[file_size + 1];
    fread(buffer, 1, file_size, file);
    buffer[file_size] = 0;
    fclose(file);

    if (stbtt_InitFont(&fontInfo, buffer, stbtt_GetFontOffsetForIndex(buffer, 0)) == 0) {
        std::cerr << "[ERROR] Can't load font: " << path << std::endl;
        exit(EXIT_FAILURE);
    }
}

Font::~Font() {
    delete buffer;
}

SDL_Texture *Font::render(SDL_Renderer *renderer, const char *text, int size, uint32_t color) {
    int length = strlen(text);

    float scale = stbtt_ScaleForPixelHeight(&fontInfo, size);

    int width = 0;
    for (int i = 0; i < length; i++) {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&fontInfo, text[i], &advance, &lsb);

        width += advance * scale;

        if (text[i + 1]) {
            width += stbtt_GetCodepointKernAdvance(&fontInfo, text[i], text[i + 1]) * scale;
        }
    }

    int height = size;

    uint8_t *bitmap = new uint8_t[height * width];
    for (int i = 0; i < height * width; i++) {
        bitmap[i] = 0;
    }

    int x = 0;

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);

    for (int i = 0; i < length; i++) {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&fontInfo, text[i], &advance, &lsb);

        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&fontInfo, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        int y = ascent + c_y1;

        int characterWidth, characterHeight;
        uint8_t *characterBitmap = stbtt_GetCodepointBitmap(&fontInfo, 0, scale, text[i], &characterWidth, &characterHeight, 0,0);

        for (int cy = 0; cy < characterHeight; cy++) {
            for (int cx = 0; cx < characterWidth; cx++) {
                int pos = (y + cy) * width + (x + cx);
                if (bitmap[pos] == 0) {
                    bitmap[pos] = characterBitmap[cy * characterWidth + cx];
                } else {
                    bitmap[pos] = std::max(bitmap[pos], characterBitmap[cy * characterWidth + cx]);
                }
            }
        }

        stbtt_FreeBitmap(characterBitmap, nullptr);

        x += advance * scale;

        if (text[i + 1]) {
            x += stbtt_GetCodepointKernAdvance(&fontInfo, text[i], text[i + 1]) * scale;
        }
    }

    uint8_t *coloredBitmap = new uint8_t[height * width * 4];
    for (int i = 0; i < height * width * 4; i++) {
        coloredBitmap[i] = 0;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pos = (y * width + x) * 4;
            coloredBitmap[pos] = bitmap[y * width + x] * (color & 0xff) / 255;
            coloredBitmap[pos + 1] = bitmap[y * width + x] * ((color >> 8) & 0xff) / 255;
            coloredBitmap[pos + 2] = bitmap[y * width + x] * ((color >> 16) & 0xff) / 255;
            coloredBitmap[pos + 3] = bitmap[y * width + x] == 0 ? 0 : 255;
        }
    }

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)coloredBitmap, width, height, 32, 4 * width,
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if (surface == nullptr) {
        std::cerr << "[ERROR] Can't create SDL surface: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cerr << "[ERROR] Can't create SDL texture: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(surface);

    return texture;
}
