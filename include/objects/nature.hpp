// VroemVroem - Nature Object Header

#pragma once

#include "objects/object.hpp"
#include <memory>
#include "image.hpp"
#include "canvas.hpp"
#include "camera.hpp"

namespace Objects {

class Nature : public Object {
    public:
        enum class Type {
            BUSHES = 0,
            BERRIES,
            STONE,
            GOLD,
            BEECH,
            BEECH_SMALL,
            FIR,
            FIR_SMALL,
            TRUNK,
            TRUNK_SMALL,
            size
        };

    private:
        static std::unique_ptr<Image> images[static_cast<size_t>(Nature::Type::size)];

        static const char *names[static_cast<size_t>(Nature::Type::size)];

        Nature::Type type;

    public:
        Nature(int id, Nature::Type type, float x, float y);

        Nature::Type getType() const;

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;

        static void loadImages(std::shared_ptr<Canvas> canvas);

        static const Image *getImage(Nature::Type type);

        static const char *getName(Nature::Type type);
};

}
