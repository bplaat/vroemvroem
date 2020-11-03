// VroemVroem - Driver Object Header

#pragma once

namespace Objects {
class Driver;
}

#include "objects/vehicle.hpp"
#include <memory>
#include "canvas.hpp"
#include "camera.hpp"

namespace Objects {

class Driver {
        friend class Vehicle;

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
            RIGHT,
            size
        };

    private:
        const Vehicle *vehicle;

        float destinationX;

        float destinationY;

        Driver::Moving moving = Driver::Moving::NOT;

        Driver::Turning turning = Driver::Turning::NOT;

        bool arrived = false;

    public:
        Driver(const Vehicle *vehicle, float destinationX, float destinationY);

        const Vehicle *getVehicle() const;

        float getDestinationX() const;

        float getDestinationY() const;

        Driver::Moving getMoving() const;

        Driver::Turning getTurning() const;

        bool isArrived() const;

        void update(float delta);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;
};

}
