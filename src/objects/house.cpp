// VroemVroem - House Object

#include "objects/house.hpp"
#include "rect.hpp"

namespace Objects {

std::unique_ptr<Image> House::images[static_cast<size_t>(House::Type::size)];

const char *House::names[static_cast<size_t>(House::Type::size)] = {
    "Standard House",
    "Small house",
    "Backery",
    "Shop"
};

House::House(int id, House::Type type, float x, float y, int population)
    : Object::Object(id, x, y), type(type), population(population) {}

House::Type House::getType() const {
    return type;
}

int House::getPopulation() const {
    return population;
}

void House::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    Rect houseRect = {
        (int)(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - tileSize / 2),
        (int)(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - tileSize / 2),
        tileSize,
        tileSize
    };

    if (canvasRect->collides(&houseRect)) {
        getImage(type)->draw(&houseRect);
    }
}

void House::loadImages(std::shared_ptr<Canvas> canvas) {
    images[static_cast<size_t>(House::Type::HOUSE)] = std::make_unique<Image>(canvas, "assets/images/houses/house1.png", true);
    images[static_cast<size_t>(House::Type::HOUSE_SMALL)] = std::make_unique<Image>(canvas, "assets/images/houses/house2.png", true);
    images[static_cast<size_t>(House::Type::BACKERY)] = std::make_unique<Image>(canvas, "assets/images/houses/house3.png", true);
    images[static_cast<size_t>(House::Type::SHOP)] = std::make_unique<Image>(canvas, "assets/images/houses/house4.png", true);
}

const Image *House::getImage(House::Type type) {
    return images[static_cast<size_t>(type)].get();
}

const char *House::getName(House::Type type) {
    return names[static_cast<size_t>(type)];
}

}
