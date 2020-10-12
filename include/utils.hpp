// VroemVroem - Utils Header

#pragma once

#define RGB(r, g, b) (uint32_t)((uint8_t)r | ((uint8_t)g << 8) | ((uint8_t)b << 16))

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
