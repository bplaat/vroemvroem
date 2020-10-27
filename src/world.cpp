// VroemVroem - World Object

#include <iostream>

#include "world.hpp"
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include "utils.hpp"
#include "noise.hpp"
#include "resources.hpp"
#include "random.hpp"
#include "object.hpp"
#include "city.hpp"
class World;
#include "camera.hpp"

World::World(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<Resources> resources, int width, int height, int seed)
    : renderer(renderer), resources(resources), width(width), height(height)
{
    random = std::make_unique<Random>(seed);

    terrainMap = std::make_unique<uint8_t[]>(height * width);
    objectMap = std::make_unique<uint8_t[]>(height * width);

    FastNoiseLite heightNoise;
    heightNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    heightNoise.SetSeed(seed);

    FastNoiseLite objectsNoise;
    objectsNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    objectsNoise.SetSeed(seed + 1);

    // Generate terrain and nature objects
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float h = heightNoise.GetNoise((float)x, (float)y);
            float o = objectsNoise.GetNoise((float)x, (float)y);

            if (h >= 0.9) {
                terrainMap[y * width + x] = random->randomInt(10, 11);
            }

            else if (h >= 0.8) {
                if (o >= 0.2 && random->randomInt(1, 6) == 1) {
                    objects.push_back(std::make_unique<Object>(objects.size(), 8, x, y));
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.2 && random->randomInt(1, 6) == 1) {
                    objects.push_back(std::make_unique<Object>(objects.size(), 9, x, y));
                    objectMap[y * width + x] = 1;
                }

                terrainMap[y * width + x] = random->randomInt(8, 9);
            }

            else if (h >= 0.7) {
                terrainMap[y * width + x] = random->randomInt(6, 7);
            }

            else if (h >= 0.1) {
                if (h >= 0.4 && o >= 0.3 && random->randomInt(1, random->randomInt(1, 2)) == 1) {
                    if (random->randomInt(1, 4) <= 3) {
                        objects.push_back(std::make_unique<Object>(objects.size(), random->randomInt(2, 5), x, y));
                    } else {
                        objects.push_back(std::make_unique<Object>(objects.size(), random->randomInt(6, 7), x, y));
                    }
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.4 && random->randomInt(1, 15) == 1) {
                    objects.push_back(std::make_unique<Object>(objects.size(), random->randomInt(0, 1), x, y));
                    objectMap[y * width + x] = 1;
                }

                terrainMap[y * width + x] = random->randomInt(4, 5);
            }

            else if (h >= -0.1) {
                terrainMap[y * width + x] = random->randomInt(2, 3);
            }

            else if (h >= -0.6) {
                terrainMap[y * width + x] = 1;
            }

            else {
                terrainMap[y * width + x] = 0;
            }
        }
    }

    // Generate cities
    for (int i = 0; i < sqrt(height * width) / 3; i++) {
        int x;
        int y;

        do {
            x = random->randomInt(0, width - 1);
            y = random->randomInt(0, height - 1);
        } while (terrainMap[y * width + x] <= 2);

        cities.push_back(std::make_unique<City>(cities.size(), std::move(City::randomName(random.get())), x, y, 0));
    }

    // Generate houses
    for (auto &city : cities) {
        int target_population = random->randomInt(50, random->randomInt(100, random->randomInt(200, random->randomInt(300, 1000))));
        int size = ceil(target_population / random->randomInt(20, random->randomInt(20, 30)));
        for (int j = 0; j < ceil(target_population / 4); j++) {
            int x;
            int y;
            int attempt = 0;
            bool foundPosition = false;
            do {
                x = city->x + random->randomInt(-size, size);
                y = city->y + random->randomInt(-size, size);

                if (
                    x >= 0 && y >= 0 && x < width && y < height &&
                    terrainMap[y * width +x] > 2 &&
                    objectMap[y * width + x] == 0
                ) {
                    foundPosition = true;
                    break;
                }

                attempt++;
            } while (!foundPosition && attempt == 10);

            if (foundPosition) {
                auto house = std::make_unique<Object>(objects.size(), random->randomInt(10, 13), x, y);
                objects.push_back(std::move(house));
                objectMap[y * width + x] = 1;
                city->population += random->randomInt(2, 6);
            }
        }
    }

    // Generate vehicles
    for (int i = 0; i < sqrt(height * width) / 3; i++) {
        int x;
        int y;

        do {
            x = random->randomInt(0, width - 1);
            y = random->randomInt(0, height - 1);
        } while (terrainMap[y * width + x] <= 2);

        vehicles.push_back(std::make_unique<Vehicle>(vehicles.size(), random->randomInt(0, 5), (float)x, (float)y, degreesToRadians(random->randomInt(0, 360))));
    }
}

void World::update(float delta) {
    (void)delta;
}

void World::draw(const Camera *camera) {
    // Get renderer size
    int gameWidth, gameHeight;
    SDL_GetRendererOutputSize(renderer.get(), &gameWidth, &gameHeight);

    // Draw terrain tiles
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Rect tileRect = {
                (int)(x * camera->tileSize - (camera->x * camera->tileSize - gameWidth / 2)),
                (int)(y * camera->tileSize - (camera->y * camera->tileSize - gameHeight / 2)),
                camera->tileSize,
                camera->tileSize
            };

            if (tileRect.x + tileRect.width >= 0 && tileRect.y + tileRect.height >= 0 && tileRect.x < gameWidth && tileRect.y < gameHeight) {
                const Image *terrainImage = resources->terrainImages[terrainMap[y * width + x]].get();
                terrainImage->draw(&tileRect);
            }
        }
    }

    // Draw objects
    for (auto &object : objects) {
        Rect objectRect = {
            (int)(object->x * camera->tileSize - (camera->x * camera->tileSize - gameWidth / 2)),
            (int)(object->y * camera->tileSize - (camera->y * camera->tileSize - gameHeight / 2)),
            camera->tileSize,
            camera->tileSize
        };

        if (objectRect.x + objectRect.width >= 0 && objectRect.y + objectRect.height >= 0 && objectRect.x < gameWidth && objectRect.y < gameHeight) {
            const Image *objectImage = resources->objectImages[object->type].get();
            objectImage->draw(&objectRect);
        }
    }

    // Draw vehicles
    for (auto &vehicle : vehicles) {
        Rect vehicleRect = {
            (int)(vehicle->x * camera->tileSize - (camera->x * camera->tileSize - gameWidth / 2)),
            (int)(vehicle->y * camera->tileSize - (camera->y * camera->tileSize - gameHeight / 2)),
            camera->tileSize,
            camera->tileSize
        };

        if (vehicleRect.x + vehicleRect.width >= 0 && vehicleRect.y + vehicleRect.height >= 0 && vehicleRect.x < gameWidth && vehicleRect.y < gameHeight) {
            const Image *vehicleImage = resources->blueVehicleImages[vehicle->type].get();
            vehicleImage->draw(&vehicleRect, vehicle->angle);
        }
    }

    // Draw city names
    for (auto &city : cities) {
        char cityLabel[128];
        sprintf(cityLabel, "%s (%d)", city->name.get(), city->population);

        Rect cityLabelRect;
        cityLabelRect.height = camera->tileSize / 2;
        cityLabelRect.width = resources->textFont->measure(cityLabel, cityLabelRect.height);
        cityLabelRect.x = (int)(city->x * camera->tileSize - (camera->x * camera->tileSize - gameWidth / 2)) - cityLabelRect.width / 2;
        cityLabelRect.y = (int)(city->y * camera->tileSize - (camera->y * camera->tileSize - gameHeight / 2)) - cityLabelRect.height / 2;

        if (cityLabelRect.x + cityLabelRect.width >= 0 && cityLabelRect.y + cityLabelRect.height >= 0 && cityLabelRect.x < gameWidth && cityLabelRect.y < gameHeight) {
            if (camera->zoomLevel >= 2) {
                // auto cityLabelImage = resources->textFont->render(renderer, cityLabel, cityLabelRect.height, RGB(255, 255, 255));
                // cityLabelImage->draw(&cityLabelRect);
            }
        }
    }
}
