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

void Canvas::clear(const Color *color) {
    SDL_SetRenderDrawColor(renderer.get(), color->red, color->blue, color->green, color->alpha);
    SDL_RenderClear(renderer.get());
}

void Canvas::fillRect(const Rect *rect, const Color *color) {
    SDL_SetRenderDrawColor(renderer.get(), color->red, color->blue, color->green, color->alpha);
    SDL_RenderFillRect(renderer.get(), (SDL_Rect *)rect);
}

void Canvas::drawTexture(SDL_Texture *texture, const Rect *rect) {
    SDL_RenderCopy(renderer.get(), texture, nullptr, (SDL_Rect *)rect);
}

void Canvas::drawTexture(SDL_Texture *texture, const Rect *rect, float angle) {
    SDL_RenderCopyEx(renderer.get(), texture, nullptr, (SDL_Rect *)rect, degrees(angle), nullptr, SDL_FLIP_NONE);
}

void Canvas::drawTexture(SDL_Texture *texture, const Rect *destinationRect, const Rect *sourceRect) {
    SDL_RenderCopy(renderer.get(), texture, (SDL_Rect *)sourceRect, (SDL_Rect *)destinationRect);
}

void Canvas::drawTexture(SDL_Texture *texture, const Rect *destinationRect, const Rect *sourceRect, float angle) {
    SDL_RenderCopyEx(renderer.get(), texture, (SDL_Rect *)sourceRect, (SDL_Rect *)destinationRect, degrees(angle), nullptr, SDL_FLIP_NONE);
}

void Canvas::present() {
    SDL_RenderPresent(renderer.get());
}
