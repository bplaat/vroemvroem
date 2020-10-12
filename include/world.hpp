// VroemVroem - World Object Header

#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include "resources.hpp"
#include "random.hpp"
#include "object.hpp"
#include "city.hpp"
class World;
#include "camera.hpp"

class World {
    public:
        std::shared_ptr<SDL_Renderer> renderer;
        std::shared_ptr<Resources> resources;
        int width;
        int height;
        std::unique_ptr<Random> random;
        std::unique_ptr<uint8_t[]> terrainMap;
        std::unique_ptr<uint8_t[]> objectMap;
        std::vector<std::unique_ptr<Object>> objects;
        std::vector<std::unique_ptr<City>> cities;

        World(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<Resources> resources, int width, int height, int seed);

        void update(float delta);

        void draw(const Camera *camera);
};
