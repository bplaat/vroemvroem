// VroemVroem - Vehicle

#include "objects/vehicle.hpp"
#include <cmath>
#include "rect.hpp"

namespace Objects {

Vehicle::Stats Vehicle::stats[static_cast<size_t>(Vehicle::Type::size)] = {
    // Standard car
    {
        "Normal Baby",  // name
        71,             // width
        131,            // height
        700,            // maxForwardVelocity
        200,            // forwardAcceleration
        -200,           // maxBackwardVelocity
        -60,            // backwardAcceleration
        radians(90)     // turningSpeed
    },

    // Mini car
    {
        "Mini",     // name
        71,         // width
        116,        // height
        700,        // maxForwardVelocity
        200,        // forwardAcceleration
        -200,       // maxBackwardVelocity
        -60,        // backwardAcceleration
        radians(90) // turningSpeed
    },

    // Sport car
    {
        "Sporty",   // name
        70,         // width
        131,        // height
        700,        // maxForwardVelocity
        200,        // forwardAcceleration
        -200,       // maxBackwardVelocity
        -60,        // backwardAcceleration
        radians(90) // turningSpeed
    },

    // Truck car
    {
        "Truck",    // name
        71,         // width
        131,        // height
        700,        // maxForwardVelocity
        200,        // forwardAcceleration
        -200,       // maxBackwardVelocity
        -60,        // backwardAcceleration
        radians(90) // turningSpeed
    },

    // Tesla car
    {
        "Tesla",    // name
        70,         // width
        121,        // height
        700,        // maxForwardVelocity
        200,        // forwardAcceleration
        -200,       // maxBackwardVelocity
        -60,        // backwardAcceleration
        radians(90) // turningSpeed
    },

    // Motor cycle car
    {
        "Motor cycle",  // name
        44,             // width
        100,            // height
        700,            // maxForwardVelocity
        200,            // forwardAcceleration
        -200,           // maxBackwardVelocity
        -60,            // backwardAcceleration
        radians(90)     // turningSpeed
    }
};

std::unique_ptr<Image> Vehicle::images[static_cast<size_t>(Vehicle::Color::size)][static_cast<size_t>(Vehicle::Type::size)];

Vehicle::Vehicle(int id, Vehicle::Type type, float x, float y, Vehicle::Color color, float angle)
    : Object::Object(id, x, y), type(type), color(color), angle(angle)
{
    velocity = 1;
}

Vehicle::Type Vehicle::getType() const {
    return type;
}

Vehicle::Color Vehicle::getColor() const {
    return color;
}

float Vehicle::getAngle() const {
    return angle;
}

int Vehicle::getVelocity() const {
    return velocity;
}

int Vehicle::getAcceleration() const {
    return acceleration;
}

void Vehicle::update(float delta) {
    int zoomedInSize = Camera::zoomLevels[Camera::zoomLevelsSize - 1];
    x -= static_cast<float>(velocity) / zoomedInSize * sin(angle) * delta;
    y -= static_cast<float>(velocity) / zoomedInSize * cos(angle) * delta;
}

void Vehicle::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    const Vehicle::Stats *stats = getStats(type);

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    Rect vehicleRect;
    int zoomedInSize = Camera::zoomLevels[Camera::zoomLevelsSize - 1];
    vehicleRect.width = static_cast<int>(static_cast<float>(stats->width) / zoomedInSize * tileSize);
    vehicleRect.height = static_cast<int>(static_cast<float>(stats->height) / zoomedInSize * tileSize);
    vehicleRect.x = static_cast<int>(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - vehicleRect.width / 2);
    vehicleRect.y = static_cast<int>(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - vehicleRect.height / 2);

    if (Rect::collision(canvasRect.get(), &vehicleRect)) {
        getImage(type, color)->draw(&vehicleRect, angle);
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
