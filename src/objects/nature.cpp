// VroemVroem - Nature Object

#include "objects/nature.hpp"
#include "rect.hpp"

namespace Objects {

std::unique_ptr<Image> Nature::images[static_cast<size_t>(Nature::Type::size)];

const char *Nature::names[static_cast<size_t>(Nature::Type::size)] = {
    "Bushses",
    "Bushses with Berries",
    "Rocks",
    "Rocks with Gold",
    "Beech Tree",
    "Small Beech Tree",
    "Fir Tree",
    "Small Fir Tree",
    "Trunk",
    "Small Trunk"
};

Nature::Nature(int id, Nature::Type type, float x, float y)
    : Object::Object(id, x, y), type(type) {}

Nature::Type Nature::getType() const {
    return type;
}

void Nature::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    Rect natureRect = {
        (int)(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - tileSize / 2),
        (int)(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - tileSize / 2),
        tileSize,
        tileSize
    };

    if (canvasRect->collides(&natureRect)) {
        getImage(type)->draw(&natureRect);
    }
}

void Nature::loadImages(std::shared_ptr<Canvas> canvas) {
    images[static_cast<size_t>(Nature::Type::BUSHES)] = std::make_unique<Image>(canvas, "assets/images/natures/bush1.png", true);
    images[static_cast<size_t>(Nature::Type::BERRIES)] = std::make_unique<Image>(canvas, "assets/images/natures/bush2.png", true);
    images[static_cast<size_t>(Nature::Type::STONE)] = std::make_unique<Image>(canvas, "assets/images/natures/rock1.png", true);
    images[static_cast<size_t>(Nature::Type::GOLD)] = std::make_unique<Image>(canvas, "assets/images/natures/rock2.png", true);
    images[static_cast<size_t>(Nature::Type::BEECH)] = std::make_unique<Image>(canvas, "assets/images/natures/tree1.png", true);
    images[static_cast<size_t>(Nature::Type::BEECH_SMALL)] = std::make_unique<Image>(canvas, "assets/images/natures/tree2.png", true);
    images[static_cast<size_t>(Nature::Type::FIR)] = std::make_unique<Image>(canvas, "assets/images/natures/tree3.png", true);
    images[static_cast<size_t>(Nature::Type::FIR_SMALL)] = std::make_unique<Image>(canvas, "assets/images/natures/tree4.png", true);
    images[static_cast<size_t>(Nature::Type::TRUNK)] = std::make_unique<Image>(canvas, "assets/images/natures/trunk1.png", true);
    images[static_cast<size_t>(Nature::Type::TRUNK_SMALL)] = std::make_unique<Image>(canvas, "assets/images/natures/trunk2.png", true);
}

const Image *Nature::getImage(Nature::Type type) {
    return images[static_cast<size_t>(type)].get();
}

const char *Nature::getName(Nature::Type type) {
    return names[static_cast<size_t>(type)];
}

}
