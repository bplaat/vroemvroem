// VroemVroem - WidgetGroup Widget Header

#pragma once

#include "widgets/widget.hpp"
#include <vector>

namespace Widgets {

class WidgetGroup : public Widget {
    protected:
        std::vector<std::unique_ptr<Widget>> widgets;

    public:
        WidgetGroup(std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback);

        bool handleEvent(const SDL_Event *event);

        void draw(std::shared_ptr<Canvas> canvas) const;

        virtual void createWidgets() = 0;
};

}
