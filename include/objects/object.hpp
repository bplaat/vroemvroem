// VroemVroem - Object Header

#pragma once

#include <memory>
#include "canvas.hpp"
#include "camera.hpp"

namespace Objects {

class Object {
    protected:
        int id;

        float x;

        float y;

        Object(int id, float x, float y);

    public:
        int getId() const;

        float getX() const;

        float getY() const;

        virtual void update(float delta);

        virtual void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const = 0;
};

}
