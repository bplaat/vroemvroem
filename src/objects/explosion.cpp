// VroemVroem - Explosion Object

#include "objects/explosion.hpp"
#include "timer.hpp"
#include "rect.hpp"

namespace Objects {

int Explosion::timerEventCodeCounter = 3;

std::unique_ptr<Image> Explosion::images[static_cast<size_t>(Explosion::Type::size)][Explosion::animationFrameCount];

const char *Explosion::names[static_cast<size_t>(Explosion::Type::size)] = {
    "Fire Explosion",
    "Smoke Explosion"
};

Explosion::Explosion(int id, Explosion::Type type, float x, float y)
    : Object::Object(id, x, y), type(type)
{
    eventCode = timerEventCodeCounter;
    timerEventCodeCounter++;
    timer = SDL_AddTimer(Explosion::animationFrameSpeed, Timer::callback, reinterpret_cast<void *>(eventCode));
}

Explosion::Type Explosion::getType() const {
    return type;
}

int Explosion::getFrame() const {
    return frame;
}

bool Explosion::isFinished() const {
    return finished;
}

int Explosion::getEventCode() const {
    return eventCode;
}

SDL_TimerID Explosion::getTimer() const {
    return timer;
}

bool Explosion::handleEvent(const SDL_Event *event) {
    if (event->type == SDL_USEREVENT && event->user.code == eventCode) {
        frame++;

        if (frame == animationFrameCount) {
            finished = true;
            SDL_RemoveTimer(timer);
        }

        return true;
    }

    return false;
}

void Explosion::draw(std::shared_ptr<Canvas> canvas, const Camera *camera) const {
    if (!finished) {
        std::unique_ptr<Rect> canvasRect = canvas->getRect();

        int tileSize = Camera::zoomLevels[camera->getZoom()];

        const Image *explosionImage = getImage(type, frame);

        Rect explosionRect;
        int zoomedInSize = Camera::zoomLevels[Camera::zoomLevelsSize - 1];
        explosionRect.width = (int)((float)explosionImage->getWidth() / zoomedInSize * tileSize);
        explosionRect.height = (int)((float)explosionImage->getHeight() / zoomedInSize * tileSize);
        explosionRect.x = (int)(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2) - explosionRect.width / 2);
        explosionRect.y = (int)(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2) - explosionRect.height / 2);

        if (canvasRect->collides(&explosionRect)) {
            explosionImage->draw(&explosionRect);
        }
    }
}

void Explosion::loadImages(std::shared_ptr<Canvas> canvas) {
    images[static_cast<size_t>(Explosion::Type::FIRE)][0] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion1.png", true);
    images[static_cast<size_t>(Explosion::Type::FIRE)][1] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion2.png", true);
    images[static_cast<size_t>(Explosion::Type::FIRE)][2] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion3.png", true);
    images[static_cast<size_t>(Explosion::Type::FIRE)][3]= std::make_unique<Image>(canvas, "assets/images/explosions/explosion4.png", true);
    images[static_cast<size_t>(Explosion::Type::FIRE)][4] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion5.png", true);

    images[static_cast<size_t>(Explosion::Type::SMOKE)][0] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion-smoke1.png", true);
    images[static_cast<size_t>(Explosion::Type::SMOKE)][1] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion-smoke2.png", true);
    images[static_cast<size_t>(Explosion::Type::SMOKE)][2] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion-smoke3.png", true);
    images[static_cast<size_t>(Explosion::Type::SMOKE)][3] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion-smoke4.png", true);
    images[static_cast<size_t>(Explosion::Type::SMOKE)][4] = std::make_unique<Image>(canvas, "assets/images/explosions/explosion-smoke5.png", true);
}

const Image *Explosion::getImage(Explosion::Type type, int frame) {
    return images[static_cast<size_t>(type)][frame].get();
}

const char *Explosion::getName(Explosion::Type type) {
    return names[static_cast<size_t>(type)];
}

}
