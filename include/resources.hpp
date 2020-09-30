// VroemVroem - Resources Singleton Object Header

#pragma once

#include <vector>
#include "image.hpp"

class Resources {
    public:
        std::vector<Image *> terrainImages;
        std::vector<Image *> objectImages;

        static Resources *getInstance();

        ~Resources();
};
