// VroemVroem - Explosion Object Header

#pragma once

#include "objects/object.hpp"
#include <memory>
#include "image.hpp"
#include "canvas.hpp"
#include "camera.hpp"

namespace Objects {

class Explosion : public Object {
    public:
        enum class Type {
            FIRE = 0,
            SMOKE,
            size
        };

    private:
        static int timerEventCodeCounter;

        static const int animationFrameCount = 5;

        static const int animationFrameSpeed = 250;

        static std::unique_ptr<Image> images[static_cast<size_t>(Explosion::Type::size)][Explosion::animationFrameCount];

        static const char *names[static_cast<size_t>(Explosion::Type::size)];

        Explosion::Type type;

        int frame = 0;

        bool finished = false;

        int eventCode;

        SDL_TimerID timer;

    public:
        Explosion(int id, Explosion::Type type, float x, float y);

        Explosion::Type getType() const;

        int getFrame() const;

        bool isFinished() const;

        int getEventCode() const;

        SDL_TimerID getTimer() const;

        bool handleEvent(const SDL_Event *event);

        void draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const;

        static void loadImages(std::shared_ptr<Canvas> canvas);

        static const Image *getImage(Explosion::Type type, int frame);

        static const char *getName(Explosion::Type type);
};

}
