// VroemVroem - Driver Object

#include "objects/driver.hpp"

namespace Objects {

Driver::Driver(const Vehicle *vehicle, float destinationX, float destinationY)
    : vehicle(vehicle), destinationX(destinationX), destinationY(destinationY) {}

const Vehicle *Driver::getVehicle() const {
    return vehicle;
}

float Driver::getDestinationX() const {
    return destinationX;
}

float Driver::getDestinationY() const {
    return destinationY;
}

Driver::Moving Driver::getMoving() const {
    return moving;
}

Driver::Turning Driver::getTurning() const {
    return turning;
}

void Driver::update(float delta) {
    (void)delta;
}


}
