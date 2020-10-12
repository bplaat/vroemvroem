// VroemVroem - Game Object Header

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "resources.hpp"
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
        uint64_t time;
        bool running = false;

        std::unique_ptr<SDL_Window, SDL_deleter> window;
        std::shared_ptr<SDL_Renderer> renderer;
        std::shared_ptr<Resources> resources;
        std::shared_ptr<World> world;
        std::unique_ptr<Camera> camera;

        Game();

        void handleEvent(const SDL_Event *event);

        void update(float delta);

        void draw();

        void start();
};
