// VroemVroem - World Object

#include <iostream>

#include "world.hpp"
#include <SDL2/SDL.h>
#include "random.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "noise.hpp"
#include "resources.hpp"

World::World(int width, int height, int seed) : width(width), height(height) {
    random = new Random(seed);

    terrainMap = new uint8_t[height * width];
    objectMap = new uint8_t[height * width];

    FastNoiseLite heightNoise;
    heightNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    heightNoise.SetSeed(seed);

    FastNoiseLite objectsNoise;
    objectsNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    objectsNoise.SetSeed(seed + 1);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float h = heightNoise.GetNoise((float)x, (float)y);
            float o = objectsNoise.GetNoise((float)x, (float)y);

            if (h >= 0.9) {
                terrainMap[y * width + x] = random->randomInt(10, 11);
            }
            else if (h >= 0.8) {
                if (o >= 0.2 && random->randomInt(1, 6) == 1) {
                    objects.push_back(new Object(objects.size(), 8, x, y));
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.2 && random->randomInt(1, 6) == 1) {
                    objects.push_back(new Object(objects.size(), 9, x, y));
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
                        objects.push_back(new Object(objects.size(), random->randomInt(2, 5), x, y));
                    } else {
                        objects.push_back(new Object(objects.size(), random->randomInt(6, 7), x, y));
                    }
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.4 && random->randomInt(1, 15) == 1) {
                    objects.push_back(new Object(objects.size(), random->randomInt(0, 1), x, y));
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
}

World::~World() {
    for (Object *object : objects) {
        delete object;
    }

    delete random;
    delete terrainMap;
    delete objectMap;
}

void World::update(float delta) {
    (void)delta;
}

void World::draw(SDL_Renderer *renderer, Camera *camera) {
    int gameWidth, gameHeight;
    SDL_GetRendererOutputSize(renderer, &gameWidth, &gameHeight);

    Resources *resources = Resources::getInstance();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_Rect tileRect = {
                (int)(x * camera->tileSize - (camera->x * camera->tileSize - gameWidth / 2)),
                (int)(y * camera->tileSize - (camera->y * camera->tileSize - gameHeight / 2)),
                camera->tileSize,
                camera->tileSize
            };

            if (tileRect.x + tileRect.w >= 0 && tileRect.y + tileRect.h >= 0 && tileRect.x < gameWidth && tileRect.y < gameHeight) {
                Image *terrainImage = resources->terrainImages[terrainMap[y * width + x]];
                terrainImage->draw(renderer, &tileRect, nullptr);
            }
        }
    }

    for (Object *object : objects) {
        SDL_Rect objectRect = {
            (int)(object->x * camera->tileSize - (camera->x * camera->tileSize - gameWidth / 2)),
            (int)(object->y * camera->tileSize - (camera->y * camera->tileSize - gameHeight / 2)),
            camera->tileSize,
            camera->tileSize
        };

        if (objectRect.x + objectRect.w >= 0 && objectRect.y + objectRect.h >= 0 && objectRect.x < gameWidth && objectRect.y < gameHeight) {
            Image *objectImage = resources->objectImages[object->type];
            objectImage->draw(renderer, &objectRect, nullptr);
        }
    }
}
