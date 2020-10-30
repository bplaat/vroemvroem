// VroemVroem - Label Widget

#include "widgets/label.hpp"
#include <cstring>
#include "game.hpp"

namespace Widgets {

Label::Label(const char *text, std::unique_ptr<Rect> rect, Font *font, int textSize, std::unique_ptr<Color> textColor,
    Label::Align align, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback)
    : Widget::Widget(std::move(rect), std::move(backgroundColor), clickCallback), font(font), textSize(textSize), textColor(std::move(textColor)), align(align)
{
    setText(text);
}

void Label::setText(const char *text) {
    if (this->text == nullptr || !strcmp(this->text, text)) {
        this->text = text;
        std::shared_ptr<Canvas> canvas = Game::getInstance()->getCanvas(); // UGLY HACK
        image = font->render(canvas, text, textSize, textColor.get());
    }
}

void Label::draw(Canvas *canvas) const {
    Widget::draw(canvas);

    if (align == Label::Align::LEFT) {
        Rect imageRect = {
            rect->x,
            rect->y + (rect->height - image->getHeight()) / 2,
            image->getWidth(),
            image->getHeight()
        };
        image->draw(&imageRect);
    }

    if (align == Label::Align::CENTER) {
        Rect imageRect = {
            rect->x + (rect->width - image->getWidth()) / 2,
            rect->y + (rect->height - image->getHeight()) / 2,
            image->getWidth(),
            image->getHeight()
        };
        image->draw(&imageRect);
    }

    if (align == Label::Align::RIGHT) {
        Rect imageRect = {
            rect->x + (rect->width - image->getWidth()),
            rect->y + (rect->height - image->getHeight()) / 2,
            image->getWidth(),
            image->getHeight()
        };
        image->draw(&imageRect);
    }
}

}
