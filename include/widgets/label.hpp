// VroemVroem - Label Header

#pragma once

#include "widgets/widget.hpp"
#include "font.hpp"
#include "color.hpp"
#include "canvas.hpp"

namespace Widgets {

class Label : public Widget {
    public:
        enum class Align {
            LEFT,
            CENTER,
            RIGHT
        };

    private:
        const char *text = nullptr;

        Font *font;

        int textSize;

        std::unique_ptr<Color> textColor;

        Label::Align align;

        std::unique_ptr<Image> image;

    public:
        Label(const char *text, std::unique_ptr<Rect> rect, Font *font, int textSize, std::unique_ptr<Color> textColor,
            Label::Align align, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback);

        void setText(const char *text);

        void draw(Canvas *canvas) const;
};

}
