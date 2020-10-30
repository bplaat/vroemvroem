// VroemVroem - WidgetGroup Widget

#include "widgets/widget_group.hpp"

namespace Widgets {

WidgetGroup::WidgetGroup(std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback)
    : Widget::Widget(std::move(rect), std::move(backgroundColor), clickCallback) {}

bool WidgetGroup::handleEvent(const SDL_Event *event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        widgets.clear();
        createWidgets();
    }

    for (std::vector<std::unique_ptr<Widgets::Widget>>::reverse_iterator it = widgets.rbegin(); it != widgets.rend(); it++) {
        if ((*it)->handleEvent(event)) {
            return true;
        }
    }

    if (Widget::handleEvent(event)) {
        return true;
    }

    return false;
}

void WidgetGroup::draw(std::shared_ptr<Canvas> canvas) const {
    Widget::draw(canvas);

    for (auto const &widget : widgets) {
        widget->draw(canvas);
    }
}

}
