// VroemVroem - World Header

#pragma once

#include <memory>
#include "random.hpp"
#include <vector>
#include "objects/nature.hpp"
#include "objects/house.hpp"
#include "objects/city.hpp"
#include "objects/road.hpp"
#include "objects/vehicle.hpp"
#include "objects/explosion.hpp"
#include <SDL2/SDL.h>
#include "camera.hpp"

class World {
    private:
        static const int vehicleTimerEventCode = 2;

        uint64_t seed;

        int width;

        int height;

        std::unique_ptr<Random> random;

        std::unique_ptr<uint8_t[]> terrainMap;

        std::unique_ptr<uint8_t[]> objectMap;

        std::vector<std::unique_ptr<Objects::Nature>> natures;

        std::vector<std::unique_ptr<Objects::House>> houses;

        std::vector<std::unique_ptr<Objects::City>> cities;

        std::vector<std::unique_ptr<Objects::Road>> roads;

        std::vector<std::unique_ptr<Objects::Vehicle>> vehicles;

        std::vector<std::unique_ptr<Objects::Explosion>> explosions;

        SDL_TimerID vehicleTimer;

    public:
        World(uint64_t seed, int width, int height);

        uint64_t getSeed() const;

        int getWidth() const;

        int getHeight() const;

        std::vector<const Objects::Nature *> getNatures() const;

        std::vector<const Objects::House *> getHouses() const;

        std::vector<const Objects::City *> getCities() const;

        std::vector<const Objects::Road *> getRoads() const;

        std::vector<const Objects::Vehicle *> getVehicles() const;

        std::vector<const Objects::Explosion *> getExplosions() const;

        bool handleEvent(const SDL_Event *event);

        void update(float delta);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;

    private:
        void addVehicle();
};
