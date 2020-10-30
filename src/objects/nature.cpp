// VroemVroem - Nature

#include "objects/nature.hpp"
#include "rect.hpp"

namespace Objects {

std::unique_ptr<Image> Nature::images[static_cast<size_t>(Nature::Type::size)];

Nature::Nature(int id, Nature::Type type, float x, float y)
    : Object::Object(id, x, y), type(type) {}

Nature::Type Nature::getType() const {
    return type;
}

void Nature::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    Rect natureRect = {
        static_cast<int>(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2)),
        static_cast<int>(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2)),
        tileSize,
        tileSize
    };

    if (canvasRect->collides(&natureRect)) {
        getImage(type)->draw(&natureRect);
    }
}

void Nature::loadImages(std::shared_ptr<Canvas> canvas) {
    images[static_cast<size_t>(Nature::Type::BUSHES)] = std::make_unique<Image>(canvas, "assets/images/nature/bush1.png", true);
    images[static_cast<size_t>(Nature::Type::BERRIES)] = std::make_unique<Image>(canvas, "assets/images/nature/bush2.png", true);
    images[static_cast<size_t>(Nature::Type::STONE)] = std::make_unique<Image>(canvas, "assets/images/nature/rock1.png", true);
    images[static_cast<size_t>(Nature::Type::GOLD)] = std::make_unique<Image>(canvas, "assets/images/nature/rock2.png", true);
    images[static_cast<size_t>(Nature::Type::BEECH)] = std::make_unique<Image>(canvas, "assets/images/nature/tree1.png", true);
    images[static_cast<size_t>(Nature::Type::BEECH_SMALL)] = std::make_unique<Image>(canvas, "assets/images/nature/tree2.png", true);
    images[static_cast<size_t>(Nature::Type::FIR)] = std::make_unique<Image>(canvas, "assets/images/nature/tree3.png", true);
    images[static_cast<size_t>(Nature::Type::FIR_SMALL)] = std::make_unique<Image>(canvas, "assets/images/nature/tree4.png", true);
    images[static_cast<size_t>(Nature::Type::TRUNK)] = std::make_unique<Image>(canvas, "assets/images/nature/trunk1.png", true);
    images[static_cast<size_t>(Nature::Type::TRUNK_SMALL)] = std::make_unique<Image>(canvas, "assets/images/nature/trunk2.png", true);
}

const Image *Nature::getImage(Nature::Type type) {
    return images[static_cast<size_t>(type)].get();
}

}
