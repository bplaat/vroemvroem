// VroemVroem - World Object Header

#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "random.hpp"
#include "object.hpp"
class World;
#include "camera.hpp"

class World {
    public:
        Random *random;
        int width;
        int height;

        uint8_t *terrainMap;
        uint8_t *objectMap;
        std::vector<Object *> objects;

        World(int width, int height, int seed);

        ~World();

        void update(float delta);

        void draw(SDL_Renderer *renderer, Camera *camera);
};
