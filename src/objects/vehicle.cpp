// VroemVroem - Vehicle Object

#include "objects/vehicle.hpp"
#include <cmath>
#include "rect.hpp"

namespace Objects {

Vehicle::Stats Vehicle::stats[static_cast<size_t>(Vehicle::Type::size)] = {
    // Standard car
    {
        "Normal Baby", // name
        71,            // width (px)
        131,           // height (px)
        1600,          // weight (kg)
        700,           // maxForwardVelocity (px/s)
        200,           // forwardAcceleration (px/s)
        -200,          // maxBackwardVelocity (px/s)
        -60,           // backwardAcceleration (px/s)
        90             // turningSpeed (deg/s)
    },

    // Mini car
    {
        "Mini", // name
        71,     // width (px)
        116,    // height (px)
        1000,   // weight (kg)
        700,    // maxForwardVelocity (px/s)
        200,    // forwardAcceleration (px/s)
        -200,   // maxBackwardVelocity (px/s)
        -60,    // backwardAcceleration (px/s)
        90      // turningSpeed (deg/s)
    },

    // Sport car
    {
        "Sporty", // name
        70,       // width (px)
        131,      // height (px)
        1250,     // weight (kg)
        700,      // maxForwardVelocity (px/s)
        200,      // forwardAcceleration (px/s)
        -200,     // maxBackwardVelocity (px/s)
        -60,      // backwardAcceleration (px/s)
        90        // turningSpeed (deg/s)
    },

    // Truck car
    {
        "Truck", // name
        71,      // width (px)
        131,     // height (px)
        15000,   // weight (kg)
        700,     // maxForwardVelocity (px/s)
        200,     // forwardAcceleration (px/s)
        -200,    // maxBackwardVelocity (px/s)
        -60,     // backwardAcceleration (px/s)
        90       // turningSpeed (deg/s)
    },

    // Tesla car
    {
        "Tesla", // name
        70,      // width (px)
        121,     // height (px)
        2000,    // weight (kg)
        700,     // maxForwardVelocity (px/s)
        200,     // forwardAcceleration (px/s)
        -200,    // maxBackwardVelocity (px/s)
        -60,     // backwardAcceleration (px/s)
        90       // turningSpeed (deg/s)
    },

    // Motor cycle
    {
        "Motor cycle", // name
        44,            // width (px)
        100,           // height (px)
        150,           // weight (kg)
        700,           // maxForwardVelocity (px/s)
        200,           // forwardAcceleration (px/s)
        -200,          // maxBackwardVelocity (px/s)
        -60,           // backwardAcceleration (px/s)
        90             // turningSpeed (deg/s)
    }
};

std::unique_ptr<Image> Vehicle::images[static_cast<size_t>(Vehicle::Color::size)][static_cast<size_t>(Vehicle::Type::size)];

Vehicle::Vehicle(int id, Vehicle::Type type, float x, float y, Vehicle::Color color, float angle)
    : Object::Object(id, x, y), type(type), color(color), angle(angle), velocity(0), acceleration(0) {}

Vehicle::Type Vehicle::getType() const {
    return type;
}

Vehicle::Color Vehicle::getColor() const {
    return color;
}

float Vehicle::getAngle() const {
    return angle;
}

float Vehicle::getVelocity() const {
    return velocity;
}

float Vehicle::getAcceleration() const {
    return acceleration;
}

const Driver *Vehicle::getDriver() const {
    return driver.get();
}

void Vehicle::setDriver(std::unique_ptr<Driver> driver) {
    this->driver = std::move(driver);
}

void Vehicle::update(float delta) {
    if (driver) {
        driver->update(delta);

        const Vehicle::Stats *stats = getStats(type);

        int zoomedInSize = Camera::zoomLevels[Camera::zoomLevelsSize - 1];

        if (driver->turning == Driver::Turning::LEFT) {
            angle += radians(stats->turningSpeed) * delta;
        }

        if (driver->turning == Driver::Turning::RIGHT) {
            angle -= radians(stats->turningSpeed) * delta;
        }

        if (driver->moving == Driver::Moving::FORWARD) {
            acceleration += 1 * delta; //((float)stats->forwardAcceleration / zoomedInSize) * delta;
        }

        if (driver->moving == Driver::Moving::BACKWARD) {
            acceleration += 1 * delta; //((float)stats->forwardAcceleration / zoomedInSize) * delta;
        }

        if (driver->moving == Driver::Moving::NOT) {
            acceleration = 0;

            if (velocity > 0) {
                velocity -= velocity * delta;
            }

            if (velocity < 0) {
                velocity -= velocity * delta;
            }
        }

        velocity += acceleration * delta;
        if (velocity > 1) {//stats->maxForwardVelocity) {
            velocity = 1;//stats->maxForwardVelocity;
        }
        if (velocity < -1) {//stats->maxBackwardVelocity) {
            velocity = -1;//stats->maxBackwardVelocity;
        }

        x -= velocity / 100 * cos(angle) * delta;
        y -= velocity / 100 * sin(angle) * delta;
    }
}

void Vehicle::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    const Vehicle::Stats *stats = getStats(type);

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    Rect vehicleRect;
    int zoomedInSize = Camera::zoomLevels[Camera::zoomLevelsSize - 1];
    vehicleRect.width = (int)((float)stats->width / zoomedInSize * tileSize);
    vehicleRect.height = (int)((float)stats->height / zoomedInSize * tileSize);
    vehicleRect.x = (int)(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - vehicleRect.width / 2);
    vehicleRect.y = (int)(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - vehicleRect.height / 2);

    if (canvasRect->collides(&vehicleRect, angle)) {
        getImage(type, color)->draw(&vehicleRect, angle - M_PI / 2);
    }

    if (driver) {
        driver->draw(canvas, camera);
    }
}

const Vehicle::Stats *Vehicle::getStats(Vehicle::Type type) {
    return &stats[static_cast<size_t>(type)];
}

void Vehicle::loadImages(std::shared_ptr<Canvas> canvas) {
    // Load black vehicle images
    images[static_cast<size_t>(Vehicle::Color::BLACK)][static_cast<size_t>(Vehicle::Type::STANDARD)] = std::make_unique<Image>(canvas, "assets/images/vehicles/black/vehicle1.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLACK)][static_cast<size_t>(Vehicle::Type::MINI)] = std::make_unique<Image>(canvas, "assets/images/vehicles/black/vehicle2.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLACK)][static_cast<size_t>(Vehicle::Type::SPORT)] = std::make_unique<Image>(canvas, "assets/images/vehicles/black/vehicle3.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLACK)][static_cast<size_t>(Vehicle::Type::TRUCK)] = std::make_unique<Image>(canvas, "assets/images/vehicles/black/vehicle4.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLACK)][static_cast<size_t>(Vehicle::Type::TESLA)] = std::make_unique<Image>(canvas, "assets/images/vehicles/black/vehicle5.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLACK)][static_cast<size_t>(Vehicle::Type::MOTOR_CYCLE)] = std::make_unique<Image>(canvas, "assets/images/vehicles/black/vehicle6.png", true);

    // Load blue vehicle images
    images[static_cast<size_t>(Vehicle::Color::BLUE)][static_cast<size_t>(Vehicle::Type::STANDARD)] = std::make_unique<Image>(canvas, "assets/images/vehicles/blue/vehicle1.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLUE)][static_cast<size_t>(Vehicle::Type::MINI)] = std::make_unique<Image>(canvas, "assets/images/vehicles/blue/vehicle2.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLUE)][static_cast<size_t>(Vehicle::Type::SPORT)] = std::make_unique<Image>(canvas, "assets/images/vehicles/blue/vehicle3.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLUE)][static_cast<size_t>(Vehicle::Type::TRUCK)] = std::make_unique<Image>(canvas, "assets/images/vehicles/blue/vehicle4.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLUE)][static_cast<size_t>(Vehicle::Type::TESLA)] = std::make_unique<Image>(canvas, "assets/images/vehicles/blue/vehicle5.png", true);
    images[static_cast<size_t>(Vehicle::Color::BLUE)][static_cast<size_t>(Vehicle::Type::MOTOR_CYCLE)] = std::make_unique<Image>(canvas, "assets/images/vehicles/blue/vehicle6.png", true);

    // Load green vehicle images
    images[static_cast<size_t>(Vehicle::Color::GREEN)][static_cast<size_t>(Vehicle::Type::STANDARD)] = std::make_unique<Image>(canvas, "assets/images/vehicles/green/vehicle1.png", true);
    images[static_cast<size_t>(Vehicle::Color::GREEN)][static_cast<size_t>(Vehicle::Type::MINI)] = std::make_unique<Image>(canvas, "assets/images/vehicles/green/vehicle2.png", true);
    images[static_cast<size_t>(Vehicle::Color::GREEN)][static_cast<size_t>(Vehicle::Type::SPORT)] = std::make_unique<Image>(canvas, "assets/images/vehicles/green/vehicle3.png", true);
    images[static_cast<size_t>(Vehicle::Color::GREEN)][static_cast<size_t>(Vehicle::Type::TRUCK)] = std::make_unique<Image>(canvas, "assets/images/vehicles/green/vehicle4.png", true);
    images[static_cast<size_t>(Vehicle::Color::GREEN)][static_cast<size_t>(Vehicle::Type::TESLA)] = std::make_unique<Image>(canvas, "assets/images/vehicles/green/vehicle5.png", true);
    images[static_cast<size_t>(Vehicle::Color::GREEN)][static_cast<size_t>(Vehicle::Type::MOTOR_CYCLE)] = std::make_unique<Image>(canvas, "assets/images/vehicles/green/vehicle6.png", true);

    // Load red vehicle images
    images[static_cast<size_t>(Vehicle::Color::RED)][static_cast<size_t>(Vehicle::Type::STANDARD)] = std::make_unique<Image>(canvas, "assets/images/vehicles/red/vehicle1.png", true);
    images[static_cast<size_t>(Vehicle::Color::RED)][static_cast<size_t>(Vehicle::Type::MINI)] = std::make_unique<Image>(canvas, "assets/images/vehicles/red/vehicle2.png", true);
    images[static_cast<size_t>(Vehicle::Color::RED)][static_cast<size_t>(Vehicle::Type::SPORT)] = std::make_unique<Image>(canvas, "assets/images/vehicles/red/vehicle3.png", true);
    images[static_cast<size_t>(Vehicle::Color::RED)][static_cast<size_t>(Vehicle::Type::TRUCK)] = std::make_unique<Image>(canvas, "assets/images/vehicles/red/vehicle4.png", true);
    images[static_cast<size_t>(Vehicle::Color::RED)][static_cast<size_t>(Vehicle::Type::TESLA)] = std::make_unique<Image>(canvas, "assets/images/vehicles/red/vehicle5.png", true);
    images[static_cast<size_t>(Vehicle::Color::RED)][static_cast<size_t>(Vehicle::Type::MOTOR_CYCLE)] = std::make_unique<Image>(canvas, "assets/images/vehicles/red/vehicle6.png", true);

    // Load yellow vehicle images
    images[static_cast<size_t>(Vehicle::Color::YELLOW)][static_cast<size_t>(Vehicle::Type::STANDARD)] = std::make_unique<Image>(canvas, "assets/images/vehicles/yellow/vehicle1.png", true);
    images[static_cast<size_t>(Vehicle::Color::YELLOW)][static_cast<size_t>(Vehicle::Type::MINI)] = std::make_unique<Image>(canvas, "assets/images/vehicles/yellow/vehicle2.png", true);
    images[static_cast<size_t>(Vehicle::Color::YELLOW)][static_cast<size_t>(Vehicle::Type::SPORT)] = std::make_unique<Image>(canvas, "assets/images/vehicles/yellow/vehicle3.png", true);
    images[static_cast<size_t>(Vehicle::Color::YELLOW)][static_cast<size_t>(Vehicle::Type::TRUCK)] = std::make_unique<Image>(canvas, "assets/images/vehicles/yellow/vehicle4.png", true);
    images[static_cast<size_t>(Vehicle::Color::YELLOW)][static_cast<size_t>(Vehicle::Type::TESLA)] = std::make_unique<Image>(canvas, "assets/images/vehicles/yellow/vehicle5.png", true);
    images[static_cast<size_t>(Vehicle::Color::YELLOW)][static_cast<size_t>(Vehicle::Type::MOTOR_CYCLE)] = std::make_unique<Image>(canvas, "assets/images/vehicles/yellow/vehicle6.png", true);
}

const Image *Vehicle::getImage(Vehicle::Type type, Vehicle::Color color) {
    return images[static_cast<size_t>(color)][static_cast<size_t>(type)].get();
}

}
