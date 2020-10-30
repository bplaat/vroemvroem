// VroemVroem - Button Widget

#include "widgets/button.hpp"

namespace Widgets {

Button::Button(const char *text, std::unique_ptr<Rect> rect, Font *font, int textSize, std::unique_ptr<Color> textColor,
    std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback)
    : Label::Label(text, std::move(rect), font, textSize, std::move(textColor), Label::Align::CENTER, std::move(backgroundColor), clickCallback) {}

}
