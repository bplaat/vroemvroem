// VroemVroem - Fonts Header

#pragma once

#include <memory>
#include "font.hpp"

class Fonts {
    private:
        static std::unique_ptr<Fonts> instance;

        std::unique_ptr<Font> titleFont;

        std::unique_ptr<Font> textFont;

    public:
        Fonts();

        static const Fonts *getInstance();

        Font *getTitleFont() const;

        Font *getTextFont() const;
};
