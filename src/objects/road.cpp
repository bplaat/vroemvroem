// VroemVroem - Road Object

#include "objects/road.hpp"
#include <cmath>
#include "rect.hpp"
#ifdef DEBUG
#include <SDL2/SDL.h>
#endif

namespace Objects {

std::unique_ptr<Image> Road::image;

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
    this->lanes = std::min(lanes, 3);
}

int Road::getSpeedLimit() const {
    return speedLimit;
}

void Road::setSpeedLimit(int speedLimit) {
    this->speedLimit = speedLimit;
}

void Road::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    float lineX = x;
    float lineY = y;
    float length = sqrt((x - endX) * (x - endX) + (y - endY) * (y - endY));
    float angle = atan2(endY - y, endX - x);

    for (int i = 0; i < ceil(length); i++) {
        for (int j = 0; j < lanes * 2; j++) {
            Rect roadPartRect;
            if (abs(x - endX) > abs(y - endY)) {
                roadPartRect = {
                    (int)(lineX * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - tileSize / 2),
                    (int)((lineY - lanes + j + 0.5) * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - tileSize / 2),
                    tileSize + 2,
                    tileSize + 2
                };
            } else {
                roadPartRect = {
                    (int)((lineX - lanes + j + 0.5) * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - tileSize / 2),
                    (int)(lineY * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - tileSize / 2),
                    tileSize + 2,
                    tileSize + 2
                };
            }

            if (canvasRect->collides(&roadPartRect, angle)) {
                getImage()->draw(&roadPartRect, angle + M_PI / 2);
            }
        }

        lineX += (endX - x) / length;
        lineY += (endY - y) / length;
    }

    #ifdef DEBUG
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
        std::unique_ptr<Color> color = std::make_unique<Color>(255, 0, 0);
        canvas->drawLine(x0, y0, x1, y1, color.get());
    }
    #endif
}

void Road::loadImage(std::shared_ptr<Canvas> canvas) {
    image = std::make_unique<Image>(canvas, "assets/images/roads/road.png", true);
}

const Image *Road::getImage() {
    return image.get();
}

}
