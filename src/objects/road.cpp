// VroemVroem - Road Object

#include "objects/road.hpp"
#include <SDL2/SDL.h>
#include "rect.hpp"
#include <cmath>

namespace Objects {

std::unique_ptr<Image> Road::images[static_cast<size_t>(Road::Edge::size)];

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
    this->lanes = std::min(lanes, 4);
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

    float lineX = x <= endX ? x : endX;
    float lineY = x <= endX ? y : endY;

    float length = sqrt((x - endX) * (x - endX) + (y - endY) * (y - endY));

    float angle = x <= endX ? atan2(endY - y, endX - x) : atan2(y - endY, x - endX);

    for (int i = 0; i < ceil(length); i++) {
        for (int j = 0; j < lanes * 2; j++) {
            Road::Edge edge;
            if (j == 0) {
                edge = Road::Edge::LEFT;
            } else if (j == lanes * 2 - 1) {
                edge = Road::Edge::RIGHT;
            } else {
                edge = Road::Edge::MIDDLE;
            }

            Rect roadPartRect = {
                (int)(lineX * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - tileSize / 2),
                (int)((lineY - lanes + j + 0.5) * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - tileSize / 2),
                tileSize,
                tileSize
            };

            if (canvasRect->collides(&roadPartRect)) { // TODO
                getImage(edge)->draw(&roadPartRect, angle + M_PI / 2);
            }
        }

        if (x <= endX) {
            lineX += (endX - x) / length;
            lineY += (endY - y) / length;
        } else {
            lineX += (x - endX) / length;
            lineY += (y - endY) / length;
        }
    }
}

void Road::loadImages(std::shared_ptr<Canvas> canvas) {
    images[static_cast<size_t>(Road::Edge::LEFT)] = std::make_unique<Image>(canvas, "assets/images/roads/road1.png", true);
    images[static_cast<size_t>(Road::Edge::MIDDLE)] = std::make_unique<Image>(canvas, "assets/images/roads/road2.png", true);
    images[static_cast<size_t>(Road::Edge::RIGHT)] = std::make_unique<Image>(canvas, "assets/images/roads/road3.png", true);
}

const Image *Road::getImage(Road::Edge edge) {
    return images[static_cast<size_t>(edge)].get();
}

}
