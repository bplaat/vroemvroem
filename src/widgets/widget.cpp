// VroemVroem - Widget

#include "widgets/widget.hpp"

namespace Widgets {

Widget::Widget(std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback)
    : rect(std::move(rect)), backgroundColor(std::move(backgroundColor)), clickCallback(clickCallback) {}

bool Widget::isVisible() const {
    return visible;
}

void Widget::setVisible(bool visible) {
    this->visible = visible;
}

bool Widget::handleEvent(const SDL_Event *event) {
    if (
        event->type == SDL_MOUSEBUTTONUP &&
        rect->containsPoint(event->button.x, event->button.y) &&
        clickCallback != nullptr
    ) {
        clickCallback();
        return true;
    }

    return false;
}

void Widget::draw(std::shared_ptr<Canvas> canvas) const {
    if (visible) {
        if (backgroundColor) {
            canvas->fillRect(rect.get(), backgroundColor.get());
        }
    }
}

}
