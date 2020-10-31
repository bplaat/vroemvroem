// VroemVroem - Camera

#include "camera.hpp"

const int Camera::zoomLevels[] = { 8, 16, 24, 32, 48, 64, 80, 96, 112, 128 };
const int Camera::zoomLevelsSize = static_cast<int>(sizeof(zoomLevels) / sizeof(zoomLevels[0]));

Camera::Camera(float x, float y, int width, int height, int zoom)
    : x(x), y(y), width(width), height(height), zoom(zoom)
{
    drag.enabled = false;
}

float Camera::getX() const {
    return x;
}

void Camera::setX(float x) {
    this->x = x;
}

float Camera::getY() const {
    return y;
}

void Camera::setY(float y) {
    this->y = y;
}

int Camera::getZoom() const {
    return zoom;
}

void Camera::setZoom(int zoom) {
    if (zoom >= 0 && zoom < Camera::zoomLevelsSize) {
        this->zoom = zoom;
    }
}

bool Camera::handleEvent(const SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        drag.enabled = true;
        drag.begin.x = x;
        drag.begin.y = y;
        drag.mouse.x = event->button.x;
        drag.mouse.y = event->button.y;
    }

    if (event->type == SDL_MOUSEMOTION) {
        if (drag.enabled) {
            int tileSize = Camera::zoomLevels[zoom];

            x = drag.begin.x - (static_cast<float>(event->button.x - drag.mouse.x) / tileSize);
            y = drag.begin.y - (static_cast<float>(event->button.y - drag.mouse.y) / tileSize);

            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x > width) x = width;
            if (y > height) y = height;
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        if (drag.enabled) {
            drag.enabled = false;
        }
    }

    if (event->type == SDL_MOUSEWHEEL) {
        if (event->wheel.y < 0) {
            if (zoom > 0) {
                zoom--;
            }
        }

        if (event->wheel.y > 0) {
            if (zoom < Camera::zoomLevelsSize - 1) {
                zoom++;
            }
        }
    }

    return false;
}
