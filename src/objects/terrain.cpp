// VroemVroem - Terrain Object

#include "objects/terrain.hpp"

namespace Objects {

std::unique_ptr<Image> Terrain::images[static_cast<size_t>(Terrain::Type::size)];

void Terrain::loadImages(std::shared_ptr<Canvas> canvas) {
    images[static_cast<size_t>(Terrain::Type::WATER_DEEP)] = std::make_unique<Image>(canvas, "assets/images/terrain/water_deep.png", false);
    images[static_cast<size_t>(Terrain::Type::WATER)] = std::make_unique<Image>(canvas, "assets/images/terrain/water.png", false);
    images[static_cast<size_t>(Terrain::Type::SAND1)] = std::make_unique<Image>(canvas, "assets/images/terrain/sand1.png", false);
    images[static_cast<size_t>(Terrain::Type::SAND2)] = std::make_unique<Image>(canvas, "assets/images/terrain/sand2.png", false);
    images[static_cast<size_t>(Terrain::Type::GRASS1)] = std::make_unique<Image>(canvas, "assets/images/terrain/grass1.png", false);
    images[static_cast<size_t>(Terrain::Type::GRASS2)] = std::make_unique<Image>(canvas, "assets/images/terrain/grass2.png", false);
    images[static_cast<size_t>(Terrain::Type::DIRT1)] = std::make_unique<Image>(canvas, "assets/images/terrain/dirt1.png", false);
    images[static_cast<size_t>(Terrain::Type::DIRT2)] = std::make_unique<Image>(canvas, "assets/images/terrain/dirt2.png", false);
    images[static_cast<size_t>(Terrain::Type::STONE1)] = std::make_unique<Image>(canvas, "assets/images/terrain/stone1.png", false);
    images[static_cast<size_t>(Terrain::Type::STONE2)] = std::make_unique<Image>(canvas, "assets/images/terrain/stone1.png", false);
    images[static_cast<size_t>(Terrain::Type::SNOW1)] = std::make_unique<Image>(canvas, "assets/images/terrain/snow1.png", false);
    images[static_cast<size_t>(Terrain::Type::SNOW2)] = std::make_unique<Image>(canvas, "assets/images/terrain/snow2.png", false);
}

const Image *Terrain::getImage(Terrain::Type type) {
    return images[static_cast<size_t>(type)].get();
}

}
