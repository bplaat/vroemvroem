// VroemVroem - Widget Header

#pragma once

#include <memory>
#include "rect.hpp"
#include "color.hpp"
#include <functional>
#include <SDL2/SDL.h>
#include "canvas.hpp"

namespace Widgets {

class Widget {
    protected:
        std::unique_ptr<Rect> rect;

        std::unique_ptr<Color> backgroundColor;

        std::function<void()> clickCallback;

        bool visible = true;

    public:
        Widget(std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback);

        bool isVisible() const;

        void setVisible(bool visible);

        virtual bool handleEvent(const SDL_Event *event);

        virtual void draw(std::shared_ptr<Canvas> canvas) const;
};

}
