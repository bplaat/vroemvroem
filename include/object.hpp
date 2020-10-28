// VroemVroem - Object Header

#pragma once

#include "canvas.hpp"
#include "camera.hpp"

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

        virtual void draw(Canvas *canvas, const Camera *camera) const = 0;
};
