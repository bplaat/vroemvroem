// VroemVroem - Button Widget Header

#pragma once

#include "widgets/label.hpp"

namespace Widgets {

class Button : public Label {
    public:
        Button(const char *text, std::unique_ptr<Rect> rect, Font *font, int textSize, std::unique_ptr<Color> textColor,
            std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback);
};

}
