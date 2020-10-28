// VroemVroem - Nature Header

#pragma once

#include "object.hpp"
#include <memory>
#include "image.hpp"
#include "canvas.hpp"
#include "camera.hpp"

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

        Nature::Type type;

    public:
        Nature(int id, Nature::Type type, float x, float y);

        Nature::Type getType() const;

        void draw(Canvas *canvas, const Camera *camera) const;

        static void loadImages(std::shared_ptr<Canvas> canvas);

        static const Image *getImage(Nature::Type type);
};
