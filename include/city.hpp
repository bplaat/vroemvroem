// VroemVroem - City Header

#pragma once

#include "object.hpp"
#include <memory>
#include "canvas.hpp"
#include "camera.hpp"
#include "random.hpp"

class City : public Object {
    private:
        const char *name;

        int population;

    public:
        City(int id, const char *name, float x, float y, int population);

        const char *getName() const;

        int getPopulation() const;

        void setPopulation(int population);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;

        static const char *randomName(Random *random);
};
