// VroemVroem - Resources Object Header

#pragma once

#include <memory>
#include <vector>
#include "font.hpp"
#include "image.hpp"

class Resources {
    public:
        std::unique_ptr<Font> headerFont;
        std::unique_ptr<Font> textFont;
        std::vector<std::unique_ptr<Image>> terrainImages;
        std::vector<std::unique_ptr<Image>> objectImages;
        std::vector<std::unique_ptr<Image>> blueVehicleImages;

        Resources(std::shared_ptr<SDL_Renderer> renderer);
};
