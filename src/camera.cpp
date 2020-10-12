// VroemVroem - Camera Object

#include "camera.hpp"
#include <memory>
#include <SDL2/SDL.h>
#include "world.hpp"

const int Camera::ZOOM_LEVELS[] = { 8, 16, 24, 32, 48, 64, 80, 96, 112, 128 };

Camera::Camera(std::shared_ptr<World> world, float x, float y, int zoomLevel)
    : world(world), x(x), y(y), zoomLevel(zoomLevel)
{
    drag.enabled = false;
    tileSize = ZOOM_LEVELS[zoomLevel];
}

void Camera::handleEvent(const SDL_Event *event) {
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

            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x > world->width) x = world->width;
            if (y > world->height) y = world->height;
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
