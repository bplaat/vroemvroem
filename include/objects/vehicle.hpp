// VroemVroem - Vehicle Object Header

#pragma once

#include "objects/object.hpp"
#include <memory>
#include "image.hpp"

namespace Objects {
class Vehicle;
}
#include "objects/driver.hpp"
#include "canvas.hpp"
#include "camera.hpp"

namespace Objects {

class Vehicle : public Object {
        friend class Driver;

    public:
        struct Stats {
            const char *name;
            int width;                // px
            int height;               // px
            int weight;               // kg
            int maxForwardVelocity;   // px/s
            int forwardAcceleration;  // px/s
            int maxBackwardVelocity;  // px/s
            int backwardAcceleration; // px/s
            int turningSpeed;         // deg/s
        };

        enum class Type {
            STANDARD = 0,
            MINI,
            SPORT,
            TRUCK,
            TESLA,
            MOTOR_CYCLE,
            size
        };

        enum class Color {
            BLACK = 0,
            BLUE,
            GREEN,
            RED,
            YELLOW,
            size
        };

    private:
        static Vehicle::Stats stats[static_cast<size_t>(Vehicle::Type::size)];

        static std::unique_ptr<Image> images[static_cast<size_t>(Vehicle::Color::size)][static_cast<size_t>(Vehicle::Type::size)];

        Vehicle::Type type;

        Vehicle::Color color;

        float angle;

        float velocity;

        float acceleration;

        std::unique_ptr<Driver> driver;

    public:
        Vehicle(int id, Vehicle::Type type, float x, float y, Vehicle::Color color, float angle);

        Vehicle::Type getType() const;

        Vehicle::Color getColor() const;

        float getAngle() const;

        float getVelocity() const;

        float getAcceleration() const;

        const Driver *getDriver() const;

        void setDriver(std::unique_ptr<Driver> driver);

        void update(float delta);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;

        static const Vehicle::Stats *getStats(Vehicle::Type type);

        static void loadImages(std::shared_ptr<Canvas> canvas);

        static const Image *getImage(Vehicle::Type type, Vehicle::Color color);
};

}
