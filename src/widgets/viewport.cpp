// VroemVroem - Viewport Widget

#include "widgets/viewport.hpp"
#include "widgets/label.hpp"
#include "widgets/inspector.hpp"
#include "game.hpp"
#include "objects/nature.hpp"
#include "objects/house.hpp"
#include "objects/city.hpp"
#include "objects/vehicle.hpp"
#include "fonts.hpp"

namespace Widgets {

Viewport::Viewport(std::shared_ptr<World> world, std::shared_ptr<Camera> camera, std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor)
    : WidgetGroup::WidgetGroup(std::move(rect), std::move(backgroundColor), nullptr), world(world), camera(camera)
{
    createWidgets();
}

bool Viewport::handleEvent(const SDL_Event *event) {
    if (WidgetGroup::handleEvent(event)) {
        return true;
    }

    if (camera->handleEvent(event)) {
        return true;
    }

    if (world->handleEvent(event)) {
        return true;
    }

    Inspector *inspector = dynamic_cast<Inspector *>(widgets.at(1).get());

    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        Game *game = Game::getInstance(); // UGLY
        int gameWidth = game->getWidth();
        int gameHeight = game->getHeight();

        int tileSize = Camera::zoomLevels[camera->getZoom()];

        // Check vehicles for mouse click
        std::vector<const Objects::Vehicle *> vehicles = world->getVehicles();
        for (auto const *vehicle : vehicles) {
            const Objects::Vehicle::Stats *vehicleStats = Objects::Vehicle::getStats(vehicle->getType());

            Rect vehicleRect;
            int zoomedInSize = Camera::zoomLevels[Camera::zoomLevelsSize - 1];
            vehicleRect.width = (int)((float)vehicleStats->width / zoomedInSize * tileSize);
            vehicleRect.height = (int)((float)vehicleStats->height / zoomedInSize * tileSize);
            vehicleRect.x = (int)(vehicle->getX() * tileSize - (camera->getX() * tileSize - gameWidth / 2) - vehicleRect.width / 2);
            vehicleRect.y = (int)(vehicle->getY() * tileSize - (camera->getY() * tileSize - gameHeight / 2) - vehicleRect.height / 2);

            if (vehicleRect.containsPoint(event->button.x, event->button.y)) {
                inspector->setObject(dynamic_cast<const Objects::Object *>(vehicle));
                return true;
            }
        }

        // Check cities for mouse click
        std::vector<const Objects::City *> cities = world->getCities();
        for (auto const *city : cities) {
            char cityLabel[128];
            sprintf(cityLabel, "%s (%d)", city->getName(), city->getPopulation());

            Rect cityRect;
            cityRect.height = tileSize / 2;
            cityRect.width = Fonts::getInstance()->getTextFont()->measure(cityLabel, cityRect.height);
            cityRect.x = (int)(city->getX() * tileSize - (camera->getX() * tileSize - gameWidth / 2)) - cityRect.width / 2;
            cityRect.y = (int)(city->getY() * tileSize - (camera->getY() * tileSize - gameHeight / 2)) - cityRect.height / 2;

            if (cityRect.containsPoint(event->button.x, event->button.y)) {
                inspector->setObject(dynamic_cast<const Objects::Object *>(city));
                return true;
            }
        }

        // Check houses for mouse click
        std::vector<const Objects::House *> houses = world->getHouses();
        for (auto const *house : houses) {
            Rect houseRect = {
                (int)(house->getX() * tileSize - (camera->getX() * tileSize - gameWidth / 2) - tileSize / 2),
                (int)(house->getY() * tileSize - (camera->getY() * tileSize - gameHeight / 2) - tileSize / 2),
                tileSize,
                tileSize
            };

            if (houseRect.containsPoint(event->button.x, event->button.y)) {
                inspector->setObject(dynamic_cast<const Objects::Object *>(house));
                return true;
            }
        }

        // Check natures for mouse click
        std::vector<const Objects::Nature *> natures = world->getNatures();
        for (auto const *nature : natures) {
            Rect natureRect = {
                (int)(nature->getX() * tileSize - (camera->getX() * tileSize - gameWidth / 2) - tileSize / 2),
                (int)(nature->getY() * tileSize - (camera->getY() * tileSize - gameHeight / 2) - tileSize / 2),
                tileSize,
                tileSize
            };

            if (natureRect.containsPoint(event->button.x, event->button.y)) {
                inspector->setObject(dynamic_cast<const Objects::Object *>(nature));
                return true;
            }
        }

        // When no clicks hide inspector
        inspector->setObject(nullptr);
    }

    // When escape is pressed hide inspector
    if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_ESCAPE) {
        inspector->setObject(nullptr);
    }

    return false;
}

void Viewport::draw(std::shared_ptr<Canvas> canvas) const {
    if (visible) {
        // Update debug label
        char debugLabelString[128];
        int tileSize = Camera::zoomLevels[camera->getZoom()];
        sprintf(debugLabelString, "camera.x = %.02f, camera.y = %.02f, camera.tileSize = %d", camera->getX(), camera->getY(), tileSize);
        Label *debugLabel = dynamic_cast<Label *>(widgets.at(0).get());
        debugLabel->setText(debugLabelString);

        // Draw widgets
        Widget::draw(canvas);

        world->draw(canvas, camera.get());

        for (auto const &widget : widgets) {
            widget->draw(canvas);
        }
    }
}

void Viewport::createWidgets() {
    // Create debug label
    widgets.push_back(std::make_unique<Label>(
        "",
        std::move(std::make_unique<Rect>(rect->x + 16, rect->y + 16, rect->width, 32)),
        Fonts::getInstance()->getTextFont(),
        32,
        std::move(std::make_unique<Color>(0, 0, 0)),
        Label::Align::LEFT,
        nullptr,
        nullptr
    ));

    // Create inspector
    widgets.push_back(std::make_unique<Inspector>(
        nullptr,
        std::move(std::make_unique<Rect>(rect->x, rect->y + (rect->height - 256), 320, 256)),
        std::move(std::make_unique<Color>(34, 34, 34)),
        nullptr
    ));
    widgets.at(1)->setVisible(false);
}

}
