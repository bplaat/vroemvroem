// VroemVroem - Canvas

#include "canvas.hpp"
#include <iostream>

Canvas::Canvas(SDL_Window *window) {
    renderer = std::unique_ptr<SDL_Renderer, SDL_deleter>(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!renderer) {
        std::cerr << "[ERROR] Can't create the SDL renderer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

SDL_Renderer *Canvas::getRenderer() const {
    return renderer.get();
}

std::unique_ptr<Rect> Canvas::getRect() const {
    std::unique_ptr<Rect> rect = std::make_unique<Rect>();
    rect->x = 0;
    rect->y = 0;
    SDL_GetRendererOutputSize(renderer.get(), &rect->width, &rect->height);
    return rect;
}
