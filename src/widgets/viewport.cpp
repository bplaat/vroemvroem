// VroemVroem - Viewport Widget

#include "widgets/viewport.hpp"
#include "widgets/label.hpp"
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

    return false;
}

void Viewport::draw(std::shared_ptr<Canvas> canvas) const {
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
}

}
