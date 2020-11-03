// VroemVroem - Camera Header

#pragma once

#include <SDL2/SDL.h>

class Camera {
    public:
        static const int zoomLevels[];
        static const int zoomLevelsSize;

    private:
        float x;

        float y;

        int width;

        int height;

        int zoom;

        int mouseX;

        int mouseY;

        struct {
            bool enabled;

            struct {
                float x;
                float y;
            } begin;

            struct {
                int x;
                int y;
            } mouse;
        } drag;

    public:
        Camera(float x, float y, int width, int height, int zoom);

        float getX() const;

        void setX(float x);

        float getY() const;

        void setY(float y);

        int getWidth() const;

        int getHeight() const;

        int getZoom() const;

        void setZoom(int zoom);

        int getMouseX() const;

        int getMouseY() const;

        bool handleEvent(const SDL_Event *event);
};
