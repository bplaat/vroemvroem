// VroemVroem - Image Object Header

#pragma once

#include <SDL2/SDL.h>

class Image {
    public:
        int width;
        int height;
        bool transparent;
        SDL_Texture *texture;

        Image(SDL_Renderer *renderer, const char *path, bool transparent);

        ~Image();
};
