// VroemVroem - Fonts

#include "fonts.hpp"

std::unique_ptr<Fonts> Fonts::instance;

Fonts::Fonts() {
    headerFont = std::make_unique<Font>("assets/fonts/Bangers-Regular.ttf");

    textFont = std::make_unique<Font>("assets/fonts/Merriweather-Regular.ttf");
}

const Fonts *Fonts::getInstance() {
    if (!instance) {
        instance = std::make_unique<Fonts>();
    }
    return instance.get();
}

Font *Fonts::getHeaderFont() const {
    return headerFont.get();
}

Font *Fonts::getTextFont() const {
    return textFont.get();
}
