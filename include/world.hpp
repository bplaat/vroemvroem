// VroemVroem - World Object Header

#pragma once

#include <SDL2/SDL.h>
class Game;
#include "game.hpp"
#include "random.hpp"
#include "camera.hpp"

class World {
    public:
        Game *game;
        Random *random;
        int width;
        int height;
        uint8_t *terrain;

        World(Game *game, int width, int height, int seed);

        ~World();

        void update(float delta);

        void draw(SDL_Renderer *renderer, Camera *camera);
};
