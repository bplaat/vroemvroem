// VroemVroem - Resources Singleton Object

#include "resources.hpp"

Resources *Resources::getInstance() {
    static Resources instance;
    return &instance;
}

Resources::~Resources() {
    for (Image *terrainImage : terrainImages) {
        delete terrainImage;
    }

    for (Image *objectImage : objectImages) {
        delete objectImage;
    }
}
