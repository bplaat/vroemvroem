// VroemVroem - World Header

#pragma once

#include <memory>
#include "random.hpp"
#include <vector>
#include "objects/nature.hpp"
#include "objects/house.hpp"
#include "objects/city.hpp"
#include "objects/vehicle.hpp"
#include "camera.hpp"

class World {
    private:
        uint64_t seed;

        int width;

        int height;

        std::unique_ptr<Random> random;

        std::unique_ptr<uint8_t[]> terrainMap;

        std::unique_ptr<uint8_t[]> objectMap;

        std::vector<std::unique_ptr<Objects::Nature>> natures;

        std::vector<std::unique_ptr<Objects::House>> houses;

        std::vector<std::unique_ptr<Objects::City>> cities;

        std::vector<std::unique_ptr<Objects::Vehicle>> vehicles;

    public:
        World(uint64_t seed, int width, int height);

        uint64_t getSeed() const;

        int getWidth() const;

        int getHeight() const;

        void update(float delta);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;
};
