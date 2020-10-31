// VroemVroem - Page

#include "pages/page.hpp"
#include "random.hpp"
#include "utils.hpp"

namespace Pages {

Page::Page() {
    Random *random = Random::getInstance();
    backgroundColor = std::make_unique<Color>(random->random(50, 150), random->random(50, 150), random->random(50, 150));
}

bool Page::handleEvent(const SDL_Event *event) {
    for (std::vector<std::unique_ptr<Widgets::Widget>>::reverse_iterator it = widgets.rbegin(); it != widgets.rend(); it++) {
        if ((*it)->handleEvent(event)) {
            return true;
        }
    }

    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        widgets.clear();
        createWidgets();
    }

    return false;
}

void Page::update(float delta) {
    (void)delta;
}

void Page::draw(std::shared_ptr<Canvas> canvas) const {
    canvas->clear(backgroundColor.get());

    for (auto const &widget : widgets) {
        widget->draw(canvas);
    }
}

}
