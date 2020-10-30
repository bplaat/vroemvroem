// VroemVroem - Color Header

#pragma once

#include <cstdint>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    Color();

    Color(uint8_t red, uint8_t green, uint8_t blue);

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
};
