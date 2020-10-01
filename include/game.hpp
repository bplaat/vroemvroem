// VroemVroem - Game Object Header

#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "world.hpp"
#include "camera.hpp"

class Game {
    public:
        const char *title = "VroemVroem";
        int width = 1280;
        int height = 720;
        int minWidth = 640;
        int minHeight = 480;
        bool fullscreen = false;
        bool running = true;

        SDL_Window *window;
        SDL_Renderer *renderer;
        uint64_t time;

        World *world;
        Camera *camera;

        SDL_Texture *textTexture;

        Game();

        ~Game();

        void handleEvent(SDL_Event *event);

        void update(float delta);

        void draw();

        void start();
};
