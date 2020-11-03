// VroemVroem - Driver Object

#include "objects/driver.hpp"
#include <cmath>

namespace Objects {

Driver::Driver(const Vehicle *vehicle, float destinationX, float destinationY)
    : vehicle(vehicle), destinationX(destinationX), destinationY(destinationY), moving(Moving::FORWARD) {}

const Vehicle *Driver::getVehicle() const {
    return vehicle;
}

float Driver::getDestinationX() const {
    return destinationX;
}

float Driver::getDestinationY() const {
    return destinationY;
}

Driver::Moving Driver::getMoving() const {
    return moving;
}

Driver::Turning Driver::getTurning() const {
    return turning;
}

void Driver::update(float delta) {
    (void)delta;

    if (
        moving == Moving::FORWARD &&
        sqrt(
            (vehicle->x - destinationX) * (vehicle->x - destinationX) +
            (vehicle->y - destinationY) * (vehicle->y - destinationY)
        ) < 2
    ) {
        moving = Moving::NOT;
    }
}

void Driver::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    #ifdef DEBUG
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    SDL_Renderer *renderer = canvas->getRenderer();

    int x0 = (int)(vehicle->x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2));
    int y0 = (int)(vehicle->y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2));
    int x1 = (int)(destinationX * tileSize - (camera->getX() * tileSize - canvasRect->width / 2));
    int y1 = (int)(destinationY * tileSize - (camera->getY() * tileSize - canvasRect->height / 2));

    Rect destinationRect;
    destinationRect.x = std::min(x0, x1);
    destinationRect.y = std::min(y0, y1);
    destinationRect.width = std::max(x0, x1) - destinationRect.x;
    destinationRect.height = std::max(y0, y1) - destinationRect.y;

    if (canvasRect->collides(&destinationRect)) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    }
    #endif
}

}
