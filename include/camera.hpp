// VroemVroem - Camera Object Header

#pragma once

#include <SDL2/SDL.h>

class Camera {
    public:
        static const int ZOOM_LEVELS[];

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

        Camera(float x, float y, int zoomLevel);

        void handleEvent(SDL_Event *event);
};

const int Camera::ZOOM_LEVELS[] = { 16, 24, 32, 48, 64, 80, 96, 112, 128 };
