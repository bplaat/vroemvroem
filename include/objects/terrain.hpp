// VroemVroem - Terrain Object Header

#pragma once

#include <memory>
#include "image.hpp"
#include "canvas.hpp"

namespace Objects {

class Terrain {
    public:
        enum class Type {
            WATER_DEEP = 0,
            WATER,
            SAND1,
            SAND2,
            GRASS1,
            GRASS2,
            DIRT1,
            DIRT2,
            STONE1,
            STONE2,
            SNOW1,
            SNOW2,
            size
        };

    private:
        static std::unique_ptr<Image> images[static_cast<size_t>(Terrain::Type::size)];

    public:
        static void loadImages(std::shared_ptr<Canvas> canvas);

        static const Image *getImage(Terrain::Type type);
};

}
