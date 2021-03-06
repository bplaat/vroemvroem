// VroemVroem - Image Widget

#include "widgets/image_widget.hpp"

namespace Widgets {

ImageWidget::ImageWidget(std::unique_ptr<Image> image, std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback)
    : Widget::Widget(std::move(rect), std::move(backgroundColor), std::move(clickCallback)), image(std::move(image)) {}

void ImageWidget::draw(std::shared_ptr<Canvas> canvas) const {
    if (visible) {
        Widget::draw(canvas);

        Rect imageRect = {
            rect->x + (rect->width - image->getWidth()) / 2,
            rect->y + (rect->height - image->getHeight()) / 2,
            image->getWidth(),
            image->getHeight()
        };
        image->draw(&imageRect);
    }
}

}
