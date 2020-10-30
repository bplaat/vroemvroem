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

    if (event->type == SDL_WINDOWEVENT) {
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            widgets.clear();
            createWidgets();
            return true;
        }
    }

    return false;
}

void Page::update(float delta) {
    (void)delta;
}

void Page::draw(Canvas *canvas) const {
    SDL_Renderer *renderer = canvas->getRenderer();

    SDL_SetRenderDrawColor(renderer, backgroundColor->red, backgroundColor->blue, backgroundColor->green, backgroundColor->alpha);
    SDL_RenderClear(renderer);

    for (auto const &widget : widgets) {
        widget->draw(canvas);
    }
}

}
