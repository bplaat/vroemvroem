// VroemVroem - Game Object Header

#pragma once

#include <SDL2/SDL.h>

class Game {
    public:
        const char *title = "VroemVroem";
        int width = 1280;
        int height = 720;
        int minWidth = 640;
        int minHeight = 480;
        bool fullscreen = false;
        SDL_Window *window;
        SDL_Renderer *renderer;
        uint64_t time;
        bool running = true;
        SDL_Texture *terrainImages[6];

        Game();

        void handleEvent(SDL_Event *event);

        void update(double delta);

        void draw();

        void start();
};
