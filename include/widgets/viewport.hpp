// VroemVroem - Viewport Widget Header

#pragma once

#include "widgets/widget_group.hpp"
#include "world.hpp"
#include "camera.hpp"

namespace Widgets {

class Viewport : public WidgetGroup {
    private:
        std::shared_ptr<World> world;

        std::shared_ptr<Camera> camera;

    public:
        Viewport(std::shared_ptr<World> world, std::shared_ptr<Camera> camera, std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor);

        bool handleEvent(const SDL_Event *event);

        void draw(std::shared_ptr<Canvas> canvas) const;

        void createWidgets();
};

}
