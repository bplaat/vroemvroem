// VroemVroem - Driver Object Header

#pragma once

namespace Objects {
class Driver;
}

#include "objects/vehicle.hpp"

namespace Objects {

class Driver {
    public:
        enum class Moving {
            NOT,
            FORWARD,
            BACKWARD,
            size
        };

        enum class Turning {
            NOT,
            LEFT,
            RIGTH,
            size
        };

    private:
        const Vehicle *vehicle;

        float destinationX;

        float destinationY;

        Driver::Moving moving = Driver::Moving::NOT;

        Driver::Turning turning = Driver::Turning::NOT;

    public:
        Driver(const Vehicle *vehicle, float destinationX, float destinationY);

        const Vehicle *getVehicle() const;

        float getDestinationX() const;

        float getDestinationY() const;

        Driver::Moving getMoving() const;

        Driver::Turning getTurning() const;

        void update(float delta);
};

}
