// VroemVroem - Inspector Widget Header

#pragma once

#include "widgets/widget_group.hpp"
#include "objects/object.hpp"

namespace Widgets {

class Inspector : public WidgetGroup {
    private:
        const Objects::Object *object;

    public:
        Inspector(const Objects::Object *object, std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback);

        const Objects::Object *getObject() const;

        void setObject(const Objects::Object *object);

        void draw(std::shared_ptr<Canvas> canvas) const;

        void createWidgets();
};

}
