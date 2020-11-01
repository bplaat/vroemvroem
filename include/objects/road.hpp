// VroemVroem - Road Object Header

#pragma once

#include "objects/object.hpp"

namespace Objects {

class Road : public Object {
    private:
        float endX;

        float endY;

        int lanes;

    public:
        Road(int id, float x, float y, float endX, float endY, int lanes);

        float getEndX() const;

        float getEndY() const;

        int getLanes() const;

        void setLanes(int lanes);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;
};

}
