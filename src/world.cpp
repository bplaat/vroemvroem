// VroemVroem - World Object

#include <iostream>

#include "world.hpp"
#include <ctime>
#include <SDL2/SDL.h>
#include "game.hpp"
#include "random.hpp"
#include "camera.hpp"

World::World(Game *game, int width, int height, int seed)
    : game(game), width(width), height(height)
{
    random = new Random(seed);

    terrain = new uint8_t[height * width];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float n = random->random();

            if (n > 0.5) {
                terrain[y * width + x] = random->randomInt(4, 5);
            }
            else if (n > 0.3) {
                terrain[y * width + x] = random->randomInt(2, 3);
            }
            else {
                terrain[y * width + x] = random->randomInt(0, 1);
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
                SDL_RenderCopy(renderer, game->terrainImages[terrain[y * width + x]]->texture, nullptr, &tileRect);
            }
        }
    }
}
