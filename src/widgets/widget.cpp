// VroemVroem - Widget

#include "widgets/widget.hpp"

namespace Widgets {

Widget::Widget(std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback)
    : rect(std::move(rect)), backgroundColor(std::move(backgroundColor)), clickCallback(clickCallback) {}

bool Widget::handleEvent(const SDL_Event *event) {
    if (
        clickCallback != nullptr &&
        event->type == SDL_MOUSEBUTTONUP &&
        rect->containsPoint(event->button.x, event->button.y)
    ) {
        clickCallback();
        return true;
    }

    return false;
}

void Widget::draw(Canvas *canvas) const {
    if (backgroundColor) {
        SDL_Renderer *renderer = canvas->getRenderer();
        SDL_SetRenderDrawColor(renderer, backgroundColor->red, backgroundColor->blue, backgroundColor->green, backgroundColor->alpha);
        SDL_RenderFillRect(renderer, (SDL_Rect *)rect.get());
    }
}

}
