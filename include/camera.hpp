// VroemVroem - Camera Object Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
class Camera;
#include "world.hpp"

class Camera {
    public:
        static const int ZOOM_LEVELS[];

        std::shared_ptr<World> world;

        float x;
        float y;

        struct {
            bool enabled;

            struct {
                int x;
                int y;
            } mouse;

            struct {
                float x;
                float y;
            } oldCamera;
        } drag;

        int zoomLevel;
        int tileSize;

        Camera(std::shared_ptr<World> world, float x, float y, int zoomLevel);

        void handleEvent(const SDL_Event *event);
};
