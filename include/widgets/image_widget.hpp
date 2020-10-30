// VroemVroem - Image Widget Header

#pragma once

#include "widgets/widget.hpp"
#include "../image.hpp"

namespace Widgets {

class ImageWidget : public Widget {
    private:
        std::unique_ptr<Image> image;

    public:
        ImageWidget(std::unique_ptr<Image> image, std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback);

        void draw(std::shared_ptr<Canvas> canvas) const;
};

}
