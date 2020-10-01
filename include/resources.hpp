// VroemVroem - Resources Singleton Object Header

#pragma once

#include <vector>
#include "font.hpp"
#include "image.hpp"

class Resources {
    public:
        Font *font;
        std::vector<Image *> terrainImages;
        std::vector<Image *> objectImages;

        static Resources *getInstance();

        ~Resources();
};
