// VroemVroem - World

#include "world.hpp"
#include "noise.hpp"
#include "objects/terrain.hpp"
#include "objects/driver.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "timer.hpp"
#include <map>
#include <cmath>

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
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float h = heightNoise.GetNoise((float)x, (float)y);
            float o = objectsNoise.GetNoise((float)x, (float)y);

            if (h >= 0.9) {
                terrainMap[y * width + x] = random->random(static_cast<int>(Objects::Terrain::Type::SNOW1), static_cast<int>(Objects::Terrain::Type::SNOW2));
            }

            else if (h >= 0.8) {
                if (o >= 0.2 && random->random(1, 6) == 1) {
                    natures.push_back(std::make_unique<Objects::Nature>(natures.size(), Objects::Nature::Type::GOLD, x + 0.5, y + 0.5));
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.2 && random->random(1, 6) == 1) {
                    natures.push_back(std::make_unique<Objects::Nature>(natures.size(), Objects::Nature::Type::STONE, x + 0.5, y + 0.5));
                    objectMap[y * width + x] = 1;
                }

                terrainMap[y * width + x] = random->random(static_cast<int>(Objects::Terrain::Type::STONE1), static_cast<int>(Objects::Terrain::Type::STONE2));
            }

            else if (h >= 0.7) {
                terrainMap[y * width + x] = random->random(static_cast<int>(Objects::Terrain::Type::DIRT1), static_cast<int>(Objects::Terrain::Type::DIRT2));
            }

            else if (h >= 0.1) {
                if (h >= 0.4 && o >= 0.3 && random->random(1, random->random(1, 2)) == 1) {
                    if (random->random(1, 4) <= 3) {
                        natures.push_back(std::make_unique<Objects::Nature>(
                            natures.size(),
                            static_cast<Objects::Nature::Type>(random->random(static_cast<int>(Objects::Nature::Type::BEECH), static_cast<int>(Objects::Nature::Type::FIR_SMALL))),
                            x + 0.5,
                            y + 0.5
                        ));
                    } else {
                        natures.push_back(std::make_unique<Objects::Nature>(
                            natures.size(),
                            static_cast<Objects::Nature::Type>(random->random(static_cast<int>(Objects::Nature::Type::TRUNK), static_cast<int>(Objects::Nature::Type::TRUNK_SMALL))),
                            x + 0.5,
                            y + 0.5
                        ));
                    }
                    objectMap[y * width + x] = 1;
                }
                else if (o < 0.4 && random->random(1, 15) == 1) {
                    natures.push_back(std::make_unique<Objects::Nature>(
                        natures.size(),
                        static_cast<Objects::Nature::Type>(random->random(static_cast<int>(Objects::Nature::Type::BUSHES), static_cast<int>(Objects::Nature::Type::BERRIES))),
                        x + 0.5,
                        y + 0.5
                    ));
                    objectMap[y * width + x] = 1;
                }

                terrainMap[y * width + x] = random->random(static_cast<int>(Objects::Terrain::Type::GRASS1), static_cast<int>(Objects::Terrain::Type::GRASS2));
            }

            else if (h >= -0.1) {
                terrainMap[y * width + x] = random->random(static_cast<int>(Objects::Terrain::Type::SAND1), static_cast<int>(Objects::Terrain::Type::SAND2));
            }

            else if (h >= -0.6) {
                terrainMap[y * width + x] = static_cast<int>(Objects::Terrain::Type::WATER);
            }

            else {
                terrainMap[y * width + x] = static_cast<int>(Objects::Terrain::Type::WATER_DEEP);
            }
        }
    }

    // Generate cities
    for (int i = 0; i < sqrt(height * width) / 3; i++) {
        int x;
        int y;

        do {
            x = random->random(0, width - 1);
            y = random->random(0, height - 1);
        } while (terrainMap[y * width + x] <= 2);

        cities.push_back(std::make_unique<Objects::City>(cities.size(), Objects::City::randomName(random.get()), x + 0.5, y + 0.5, 0));
    }

    // Generate houses
    for (auto &city : cities) {
        int target_population = random->random(50, random->random(100, random->random(200, random->random(300, 1000))));
        int spread = ceil(target_population / random->random(20, random->random(20, 30)));

        for (int j = 0; j < ceil(target_population / 4); j++) {
            int x;
            int y;
            int attempt = 0;
            do {
                x = (int)city->getX() + random->random(-spread, spread);
                y = (int)city->getY() + random->random(-spread, spread);
                attempt++;
            } while (!(
                attempt == spread ||
                (
                    x >= 0 && y >= 0 && x < width && y < height &&
                    terrainMap[y * width + x] >= static_cast<int>(Objects::Terrain::Type::SAND1) &&
                    objectMap[y * width + x] == 0
                )
            ));
            if (attempt == spread) {
                continue;
            }

            int population = random->random(2, 6);
            houses.push_back(std::make_unique<Objects::House>(
                houses.size(),
                static_cast<Objects::House::Type>(random->random(static_cast<int>(Objects::House::Type::HOUSE), static_cast<int>(Objects::House::Type::SHOP))),
                x + 0.5,
                y + 0.5,
                population
            ));
            objectMap[y * width + x] = 1;
            city->setPopulation(city->getPopulation() + population);
        }
    }

    // Generate roads
    std::map<int, int> cityCount;

    for (size_t i = 0; i < cities.size() / 8; i++) {
        Objects::City *city = cities.at(random->random(0, cities.size() - 1)).get();

        int lanes = random->random(1, random->random(1, 3));

        for (size_t j = 0; j < cities.size() / 8; j++) {
            Objects::City *otherCity;
            size_t attempt = 0;
            do {
                otherCity = cities.at(random->random(0, cities.size() - 1)).get();
                attempt++;
            } while (!(
                attempt == cities.size() * 2 ||
                (
                    sqrt(
                        (city->getX() - otherCity->getX()) * (city->getX() - otherCity->getX()) +
                        (city->getY() - otherCity->getY()) * (city->getY() - otherCity->getY())
                    ) < 75 &&
                    cityCount[otherCity->getId()] < 2
                )
            ));
            if (attempt == cities.size() * 2) {
                break;
            }

            roads.push_back(std::make_unique<Objects::Road>(
                roads.size(),
                city->getX(),
                city->getY(),
                otherCity->getX(),
                otherCity->getY(),
                lanes,
                random->random(60, 120)
            ));

            cityCount[city->getId()]++;
            cityCount[otherCity->getId()]++;

            city = otherCity;
        }
    }

    // Create start vehicles
    for (size_t i = 0; i < cities.size(); i++) {
        addVehicle();
    }

    // Start vehicle timer
    vehicleTimer = SDL_AddTimer(Config::vehicleTimeout, Timer::callback, reinterpret_cast<void *>(vehicleTimerEventCode));
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

std::vector<const Objects::Nature *> World::getNatures() const {
    std::vector<const Objects::Nature *> naturePointers;
    for (auto const &nature : natures) {
        naturePointers.push_back(nature.get());
    }
    return naturePointers;
}

std::vector<const Objects::House *> World::getHouses() const {
    std::vector<const Objects::House *> housePointers;
    for (auto const &house : houses) {
        housePointers.push_back(house.get());
    }
    return housePointers;
}

std::vector<const Objects::City *> World::getCities() const {
    std::vector<const Objects::City *> cityPointers;
    for (auto const &city : cities) {
        cityPointers.push_back(city.get());
    }
    return cityPointers;
}

std::vector<const Objects::Road *> World::getRoads() const {
    std::vector<const Objects::Road *> roadPointers;
    for (auto const &road : roads) {
        roadPointers.push_back(road.get());
    }
    return roadPointers;
}

std::vector<const Objects::Vehicle *> World::getVehicles() const {
    std::vector<const Objects::Vehicle *> vehiclePointers;
    for (auto const &vehicle : vehicles) {
        vehiclePointers.push_back(vehicle.get());
    }
    return vehiclePointers;
}

std::vector<const Objects::Explosion *> World::getExplosions() const {
    std::vector<const Objects::Explosion *> explosionPointers;
    for (auto const &explosion : explosions) {
        explosionPointers.push_back(explosion.get());
    }
    return explosionPointers;
}

bool World::handleEvent(const SDL_Event *event) {
    // Pass event to explosions
    for (auto const &explosion : explosions) {
        if (explosion->handleEvent(event)) {
            return true;
        }
    }

    // Create vehicle on timer
    if (event->type == SDL_USEREVENT && event->user.code == vehicleTimerEventCode) {
        for (size_t i = 0; i < cities.size() / 4; i++) {
            addVehicle();
        }

        return true;
    }

    return false;
}

void World::update(float delta) {
    // Update vehicles
    for (auto &vehicle : vehicles) {
        vehicle->update(delta);
    }

    // Check collision
    for (auto &vehicle : vehicles) {
        if (vehicle->getDriver() && !vehicle->getDriver()->isArrived()) {
            for (auto &otherVehicle : vehicles) {
                if (vehicle->getId() != otherVehicle->getId()) {
                    if (otherVehicle->getDriver() && !otherVehicle->getDriver()->isArrived()) {
                        if (
                            sqrt(
                                (vehicle->getX() - otherVehicle->getX()) * (vehicle->getX() - otherVehicle->getX()) +
                                (vehicle->getY() - otherVehicle->getY()) * (vehicle->getY() - otherVehicle->getY())
                            ) < 1
                        ) {
                            vehicle->crash();
                            otherVehicle->crash();

                            explosions.push_back(std::make_unique<Objects::Explosion>(
                                explosions.size(),
                                static_cast<Objects::Explosion::Type>(random->random(static_cast<int>(Objects::Explosion::Type::FIRE), static_cast<int>(Objects::Explosion::Type::SMOKE))),
                                vehicle->getX(),
                                vehicle->getY()
                            ));
                        }
                    }
                }
            }
        }
    }
}

void World::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    // Draw terrain tiles
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tileSize = Camera::zoomLevels[camera->getZoom()];

            Rect tileRect = {
                (int)(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2)),
                (int)(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2)),
                tileSize,
                tileSize
            };

            if (canvasRect->collides(&tileRect)) {
                Objects::Terrain::getImage(static_cast<Objects::Terrain::Type>(terrainMap[y * width + x]))->draw(&tileRect);
            }
        }
    }

    // Draw natures
    for (auto const &nature : natures) {
        nature->draw(canvas, camera);
    }

    // Draw houses
    for (auto const &house : houses) {
        house->draw(canvas, camera);
    }

    // Draw roads
    for (auto const &road : roads) {
        road->draw(canvas, camera);
    }

    // Draw not arrived vehicles
    for (auto const &vehicle : vehicles) {
        const Objects::Driver *driver = vehicle->getDriver();
        if (driver && !driver->isArrived()) {
            vehicle->draw(canvas, camera);
        }
    }

    // Draw explosions
    for (auto const &explosion : explosions) {
        explosion->draw(canvas, camera);
    }

    // Draw cities
    for (auto const &city : cities) {
        city->draw(canvas, camera);
    }
}

void World::addVehicle() {
    Objects::City *city = cities.at(random->random(0, cities.size() - 1)).get();
    float x = 0;
    float y = 0;
    float destinationX = 0;
    float destinationY = 0;
    int lanes;

    for (const auto &road : roads) {
        if (random->random(1, 2) == 1 && road->getX() == city->getX() && road->getY() == city->getY()) {
            x = road->getX() + 0.5;
            y = road->getY() + 0.5;
            destinationX = road->getEndX() + 0.5;
            destinationY = road->getEndY() + 0.5;
            lanes = road->getLanes();
            break;
        }

        if (random->random(1, 2) == 1 && road->getEndX() == city->getX() && road->getEndY() == city->getY()) {
            x = road->getEndX() + 0.5;
            y = road->getEndY() + 0.5;
            destinationX = road->getX() + 0.5;
            destinationY = road->getY() + 0.5;
            lanes = road->getLanes();
            break;
        }
    }

    if (destinationX == 0) {
        return;
    }

    if (abs(x - destinationX) > abs(y - destinationY)) {
        y -= lanes - random->random(0, lanes);
        destinationY -= lanes - random->random(0, lanes);
    } else {
        x -= lanes - random->random(0, lanes);
        destinationX -= lanes - random->random(0, lanes);
    }

    float angle = atan2(y - destinationY, x - destinationX);

    std::unique_ptr<Objects::Vehicle> vehicle = std::make_unique<Objects::Vehicle>(
        vehicles.size(),
        static_cast<Objects::Vehicle::Type>(random->random(static_cast<int>(Objects::Vehicle::Type::STANDARD), static_cast<int>(Objects::Vehicle::Type::MOTOR_CYCLE))),
        x,
        y,
        static_cast<Objects::Vehicle::Color>(random->random(static_cast<int>(Objects::Vehicle::Color::BLACK), static_cast<int>(Objects::Vehicle::Color::YELLOW))),
        angle
    );
    vehicle->setDriver(std::move(std::make_unique<Objects::Driver>(vehicle.get(), destinationX, destinationY)));
    vehicles.push_back(std::move(vehicle));
}
