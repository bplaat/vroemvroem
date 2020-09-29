// VroemVroem - Game Object Header

#pragma once

#include <SDL2/SDL.h>
class World;
#include "image.hpp"
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

        Image *terrainImages[6];
        World *world;
        Camera *camera;

        Game();

        ~Game();

        void handleEvent(SDL_Event *event);

        void update(float delta);

        void draw();

        void start();
};
