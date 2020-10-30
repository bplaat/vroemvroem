// VroemVroem - Page Header

#pragma once

#include <memory>
#include "color.hpp"
#include <vector>
#include "widgets/widget.hpp"
#include <SDL2/SDL.h>
#include "canvas.hpp"

namespace Pages {

class Page {
    protected:
        std::unique_ptr<Color> backgroundColor;

        std::vector<std::unique_ptr<Widgets::Widget>> widgets;

    public:
        Page();

        virtual bool handleEvent(const SDL_Event *event);

        virtual void createWidgets() = 0;

        virtual void update(float delta);

        virtual void draw(Canvas *canvas) const;
};

}
