// VroemVroem - Road Object Header

#pragma once

#include "objects/object.hpp"

namespace Objects {

class Road : public Object {
    private:
        float endX;

        float endY;

        int lanes;

        int speedLimit;

    public:
        Road(int id, float x, float y, float endX, float endY, int lanes, int speedLimit);

        float getEndX() const;

        float getEndY() const;

        int getLanes() const;

        void setLanes(int lanes);

        int getSpeedLimit() const;

        void setSpeedLimit(int speedLimit);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;
};

}
