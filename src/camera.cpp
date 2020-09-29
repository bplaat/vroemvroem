// VroemVroem - Camera Object

#include "camera.hpp"
#include <SDL2/SDL.h>

const int Camera::ZOOM_LEVELS[] = { 8, 16, 24, 32, 48, 64, 80, 96, 112, 128 };

Camera::Camera(float x, float y, int zoomLevel)
    : x(x), y(y), zoomLevel(zoomLevel)
{
    tileSize = ZOOM_LEVELS[zoomLevel];
}

void Camera::handleEvent(SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        drag.enabled = true;
        drag.mouse.x = event->button.x;
        drag.mouse.y = event->button.y;
        drag.oldCamera.x = x;
        drag.oldCamera.y = y;
    }

    if (event->type == SDL_MOUSEMOTION) {
        if (drag.enabled) {
            x = drag.oldCamera.x - ((float)(event->button.x - drag.mouse.x) / tileSize);
            y = drag.oldCamera.y - ((float)(event->button.y - drag.mouse.y) / tileSize);
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        if (drag.enabled) {
            drag.enabled = false;
        }
    }

    if (event->type == SDL_MOUSEWHEEL) {

        if (event->wheel.y < 0) {
            if (zoomLevel > 0) {
                tileSize = ZOOM_LEVELS[--zoomLevel];
            }
        }

        if (event->wheel.y > 0) {
            if (zoomLevel < (int)(sizeof(ZOOM_LEVELS) / sizeof(int)) - 1) {
                tileSize = ZOOM_LEVELS[++zoomLevel];
            }
        }
    }
}
