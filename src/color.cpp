// VroemVroem - Color

#include "color.hpp"

Color::Color()
    : red(0), green(0), blue(0), alpha(255) {}

Color::Color(uint8_t red, uint8_t green, uint8_t blue)
    : red(red), green(green), blue(blue), alpha(255) {}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    : red(red), green(green), blue(blue), alpha(alpha) {}
