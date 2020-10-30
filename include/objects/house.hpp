// VroemVroem - House Object Header

#pragma once

#include "objects/object.hpp"
#include <memory>
#include "image.hpp"
#include "canvas.hpp"
#include "camera.hpp"

namespace Objects {

class House : public Object {
    public:
        enum class Type {
            HOUSE = 0,
            HOUSE_SMALL,
            BACKERY,
            SHOP,
            size
        };

    private:
        static std::unique_ptr<Image> images[static_cast<size_t>(House::Type::size)];

        House::Type type;

        int population;

    public:
        House(int id, House::Type type, float x, float y, int population);

        House::Type getType() const;

        int getPopulation() const;

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;

        static void loadImages(std::shared_ptr<Canvas> canvas);

        static const Image *getImage(House::Type type);
};

}
