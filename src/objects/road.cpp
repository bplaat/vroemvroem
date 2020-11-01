// VroemVroem - Road Object

#include "objects/road.hpp"
#include <SDL2/SDL.h>
#include "rect.hpp"

namespace Objects {

Road::Road(int id, float x, float y, float endX, float endY, int lanes, int speedLimit)
    : Object::Object(id, x, y), endX(endX), endY(endY), lanes(lanes), speedLimit(speedLimit) {}

float Road::getEndX() const {
    return endX;
}

float Road::getEndY() const {
    return endY;
}

int Road::getLanes() const {
    return lanes;
}

void Road::setLanes(int lanes) {
    this->lanes = lanes;
}

int Road::getSpeedLimit() const {
    return speedLimit;
}

void Road::setSpeedLimit(int speedLimit) {
    this->speedLimit = speedLimit;
}

void Road::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    SDL_Renderer *renderer = canvas->getRenderer();

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    int x0 = (int)(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2));
    int y0 = (int)(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2));
    int x1 = (int)(endX * tileSize - (camera->getX() * tileSize - canvasRect->width / 2));
    int y1 = (int)(endY * tileSize - (camera->getY() * tileSize - canvasRect->height / 2));

    Rect roadRect;
    roadRect.x = std::min(x0, x1);
    roadRect.y = std::min(y0, y1);
    roadRect.width = std::max(x0, x1) - roadRect.x;
    roadRect.height = std::max(y0, y1) - roadRect.y;

    if (canvasRect->collides(&roadRect)) {
        SDL_SetRenderDrawColor(renderer, 106, 106, 106, 255);
        SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    }
}

}
