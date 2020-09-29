// VroemVroem - World Object

#include <iostream>

#include "world.hpp"
#include <ctime>
#include <SDL2/SDL.h>
#include "game.hpp"
#include "random.hpp"
#include "camera.hpp"
#include "noise.hpp"

World::World(Game *game, int width, int height, int seed)
    : game(game), width(width), height(height)
{
    random = new Random(seed);

    terrain = new uint8_t[height * width];

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetSeed(seed);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float n = noise.GetNoise((float)x, (float)y);

            if (n >= 0.9) {
                terrain[y * width + x] = random->randomInt(10, 11);
            }
            else if (n >= 0.8) {
                terrain[y * width + x] = random->randomInt(8, 9);
            }
            else if (n >= 0.7) {
                terrain[y * width + x] = random->randomInt(6, 7);
            }
            else if (n >= 0.2) {
                terrain[y * width + x] = random->randomInt(4, 5);
            }
            else if (n >= 0) {
                terrain[y * width + x] = random->randomInt(2, 3);
            }
            else if (n >= -0.5) {
                terrain[y * width + x] = 1;
            }
            else {
                terrain[y * width + x] = 0;
            }
        }
    }
}

World::~World() {
    delete random;
    delete terrain;
}

void World::update(float delta) {
    (void)delta;
}

void World::draw(SDL_Renderer *renderer, Camera *camera) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_Rect tileRect = {
                (int)(x * camera->tileSize - (camera->x * camera->tileSize - game->width / 2)),
                (int)(y * camera->tileSize - (camera->y * camera->tileSize - game->height / 2)),
                camera->tileSize,
                camera->tileSize
            };

            if (tileRect.x + tileRect.w >= 0 && tileRect.y + tileRect.h >= 0 && tileRect.x < game->width && tileRect.y < game->height) {
                Image *terrainImage = game->terrainImages[terrain[y * width + x]];
                if (terrainImage != nullptr) { // BUG
                    SDL_RenderCopy(renderer, terrainImage->texture, nullptr, &tileRect);
                }
            }
        }
    }
}
