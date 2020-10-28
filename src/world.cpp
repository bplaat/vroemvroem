// VroemVroem - World

#include "world.hpp"
#include "noise.hpp"
#include "random.hpp"
#include "terrain.hpp"
#include "nature.hpp"
#include "house.hpp"
#include "city.hpp"
#include "camera.hpp"
#include "utils.hpp"

World::World(uint64_t seed, int width, int height)
    : seed(seed), width(width), height(height)
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

    // Generate terrain and natures
    for (float y = 0; y < height; y++) {
        for (float x = 0; x < width; x++) {
            float h = heightNoise.GetNoise(x, y);
            float o = objectsNoise.GetNoise(x, y);

            if (h >= 0.9) {
                terrainMap[y * width + x] = random->random(static_cast<int>(Terrain::Type::SNOW1), static_cast<int>(Terrain::Type::SNOW2));
            }

            else if (h >= 0.8) {
                if (o >= 0.2 && random->random(1, 6) == 1) {
                    natures.push_back(std::make_unique<Nature>(natures.size(), Nature::Type::GOLD, x, y));
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.2 && random->random(1, 6) == 1) {
                    natures.push_back(std::make_unique<Nature>(natures.size(), Nature::Type::STONE, x, y));
                    objectMap[y * width + x] = 1;
                }

                terrainMap[y * width + x] = random->random(static_cast<int>(Terrain::Type::STONE1), static_cast<int>(Terrain::Type::STONE2));
            }

            else if (h >= 0.7) {
                terrainMap[y * width + x] = random->random(static_cast<int>(Terrain::Type::DIRT1), static_cast<int>(Terrain::Type::DIRT2));
            }

            else if (h >= 0.1) {
                if (h >= 0.4 && o >= 0.3 && random->random(1, random->random(1, 2)) == 1) {
                    if (random->random(1, 4) <= 3) {
                        natures.push_back(std::make_unique<Nature>(
                            natures.size(),
                            static_cast<Nature::Type>(random->random(static_cast<int>(Nature::Type::BEECH), static_cast<int>(Nature::Type::FIR_SMALL))),
                            x,
                            y
                        ));
                    } else {
                        natures.push_back(std::make_unique<Nature>(
                            natures.size(),
                            static_cast<Nature::Type>(random->random(static_cast<int>(Nature::Type::TRUNK), static_cast<int>(Nature::Type::TRUNK_SMALL))),
                            x,
                            y
                        ));
                    }
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.4 && random->random(1, 15) == 1) {
                    natures.push_back(std::make_unique<Nature>(
                        natures.size(),
                        static_cast<Nature::Type>(random->random(static_cast<int>(Nature::Type::BUSHES), static_cast<int>(Nature::Type::BERRIES))),
                        x,
                        y
                    ));
                    objectMap[y * width + x] = 1;
                }

                terrainMap[y * width + x] = random->random(static_cast<int>(Terrain::Type::GRASS1), static_cast<int>(Terrain::Type::GRASS2));
            }

            else if (h >= -0.1) {
                terrainMap[y * width + x] = random->random(static_cast<int>(Terrain::Type::SAND1), static_cast<int>(Terrain::Type::SAND2));
            }

            else if (h >= -0.6) {
                terrainMap[y * width + x] = static_cast<int>(Terrain::Type::WATER);
            }

            else {
                terrainMap[y * width + x] = static_cast<int>(Terrain::Type::WATER_DEEP);
            }
        }
    }

    // Generate cities
    for (int i = 0; i < sqrt(height * width) / 3; i++) {
        float x;
        float y;

        do {
            x = random->random(0, width - 1);
            y = random->random(0, height - 1);
        } while (terrainMap[y * width + x] <= 2);

        cities.push_back(std::make_unique<City>(cities.size(), City::randomName(random.get()), x, y, 0));
    }

    // Generate houses
    for (auto &city : cities) {
        int target_population = random->random(50, random->random(100, random->random(200, random->random(300, 1000))));
        int spread = ceil(target_population / random->random(20, random->random(20, 30)));
        for (int j = 0; j < ceil(target_population / 4); j++) {
            float x;
            float y;
            int attempt = 0;
            bool foundPosition = false;
            do {
                x = city->getX() + random->random(-spread, spread);
                y = city->getY() + random->random(-spread, spread);

                if (
                    x >= 0 && y >= 0 && x < width && y < height &&
                    terrainMap[y * width +x] >= static_cast<int>(Terrain::Type::SAND1) &&
                    objectMap[y * width + x] == 0
                ) {
                    foundPosition = true;
                    break;
                }

                attempt++;
            } while (!foundPosition && attempt == 10);

            if (foundPosition) {
                int population = random->random(2, 6);
                std::unique_ptr<House> house = std::make_unique<House>(
                    houses.size(),
                    static_cast<House::Type>(random->random(static_cast<int>(House::Type::HOUSE), static_cast<int>(House::Type::SHOP))),
                    x,
                    y,
                    population
                );
                houses.push_back(std::move(house));
                objectMap[y * width + x] = 1;
                city->setPopulation(city->getPopulation() + population);
            }
        }
    }

    // Generate vehicles
    for (int i = 0; i < sqrt(height * width) / 3; i++) {
        float x;
        float y;

        do {
            x = random->random(0, width - 1);
            y = random->random(0, height - 1);
        } while (terrainMap[y * width + x] <= 2);

        vehicles.push_back(std::make_unique<Vehicle>(
            vehicles.size(),
            static_cast<Vehicle::Type>(random->random(static_cast<int>(Vehicle::Type::STANDARD), static_cast<int>(Vehicle::Type::MOTOR_CYCLE))),
            x,
            y,
            static_cast<Vehicle::Color>(random->random(static_cast<int>(Vehicle::Color::BLACK), static_cast<int>(Vehicle::Color::YELLOW))),
            radians(random->random(0, 360))
        ));
    }
}

uint64_t World::getSeed() const {
    return seed;
}

int World::getWidth() const {
    return width;
}

int World::getHeight() const {
    return height;
}

void World::update(float delta) {
    // Update vehicles
    for (auto &vehicle : vehicles) {
        vehicle->update(delta);
    }
}

void World::draw(Canvas *canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    // Draw terrain tiles
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tileSize = Camera::zoomLevels[camera->getZoom()];

            Rect tileRect = {
                static_cast<int>(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2)),
                static_cast<int>(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2)),
                tileSize,
                tileSize
            };

            if (Rect::collision(canvasRect.get(), &tileRect)) {
                Terrain::getImage(static_cast<Terrain::Type>(terrainMap[y * width + x]))->draw(&tileRect);
            }
        }
    }

    // Draw natures
    for (auto &nature : natures) {
        nature->draw(canvas, camera);
    }

    // Draw houses
    for (auto &house : houses) {
        house->draw(canvas, camera);
    }

    // Draw cities
    for (auto &city : cities) {
        city->draw(canvas, camera);
    }

    // Draw vehicles
    for (auto &vehicle : vehicles) {
        vehicle->draw(canvas, camera);
    }
}
