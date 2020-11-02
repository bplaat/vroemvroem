// VroemVroem - Road Object

#include "objects/road.hpp"
#include <cmath>
#include "rect.hpp"
#ifdef DEBUG
#include <SDL2/SDL.h>
#endif

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

    float lineX = x <= endX ? x : endX;
    float lineY = x <= endX ? y : endY;

    float length = sqrt((x - endX) * (x - endX) + (y - endY) * (y - endY));

    float angle = atan2(endY - y, endX - x);

    for (int i = 0; i < ceil(length); i++) {
        for (int j = 0; j < lanes * 2; j++) {
            Road::Edge edge;
            if (abs(x - endX) > abs(y - endY)) {
                if (j == 0) {
                    edge = Road::Edge::RIGHT;
                } else if (j == lanes * 2 - 1) {
                    edge = Road::Edge::LEFT;
                } else {
                    edge = Road::Edge::MIDDLE;
                }
            } else {
                if (j == 0) {
                    edge = Road::Edge::LEFT;
                } else if (j == lanes * 2 - 1) {
                    edge = Road::Edge::RIGHT;
                } else {
                    edge = Road::Edge::MIDDLE;
                }
            }

            Rect roadPartDestinationRect;
            if (abs(x - endX) > abs(y - endY)) {
                roadPartDestinationRect = {
                    (int)(lineX * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - tileSize / 2),
                    (int)((lineY - lanes + j + 0.5) * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - tileSize / 2),
                    tileSize, // + 2, // Fix for pixel gaps
                    tileSize
                };
            } else {
                roadPartDestinationRect = {
                    (int)((lineX - lanes + j + 0.5) * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - tileSize / 2),
                    (int)(lineY * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - tileSize / 2),
                    tileSize,
                    tileSize //+ 2 // Fix for pixel gaps
                };
            }

            if (canvasRect->collides(&roadPartDestinationRect, angle)) {
                getImage(edge)->draw(&roadPartDestinationRect, angle + M_PI / 2);
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
        SDL_Renderer *renderer = canvas->getRenderer();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    }
    #endif
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
