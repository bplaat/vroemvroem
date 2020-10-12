// VroemVroem - Resources

#include "resources.hpp"
#include <memory>
#include "font.hpp"
#include "image.hpp"

Resources::Resources(std::shared_ptr<SDL_Renderer> renderer) {
    // Load font
    headerFont = std::make_unique<Font>("assets/fonts/Bangers-Regular.ttf");
    textFont = std::make_unique<Font>("assets/fonts/Merriweather-Regular.ttf");

    // Load terrain images
    /*  0 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/water_deep.png", false));

    /*  1 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/water.png", false));

    /*  2 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/sand1.png", false));
    /*  3 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/sand2.png", false));

    /*  4 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/grass1.png", false));
    /*  5 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/grass2.png", false));

    /*  6 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/dirt1.png", false));
    /*  7 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/dirt2.png", false));

    /*  8 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/stone1.png", false));
    /*  9 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/stone1.png", false));

    /* 10 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/snow1.png", false));
    /* 11 */ terrainImages.push_back(std::make_unique<Image>(renderer, "assets/images/terrain/snow2.png", false));

    // Load object images
    /*  0 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/bush1.png", true));
    /*  1 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/bush2.png", true));

    /*  2 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/tree1.png", true));
    /*  3 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/tree2.png", true));
    /*  4 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/tree3.png", true));
    /*  5 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/tree4.png", true));

    /*  6 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/trunk1.png", true));
    /*  7 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/trunk2.png", true));

    /*  8 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/rock1.png", true));
    /*  9 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/rock2.png", true));

    /* 10 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/house1.png", true));
    /* 11 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/house2.png", true));
    /* 12 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/house3.png", true));
    /* 13 */ objectImages.push_back(std::make_unique<Image>(renderer, "assets/images/objects/house4.png", true));

    // Load vehicle images
    /* 0 */ blueVehicleImages.push_back(std::make_unique<Image>(renderer, "assets/images/vehicles/blue/vehicle1.png", true));
    /* 1 */ blueVehicleImages.push_back(std::make_unique<Image>(renderer, "assets/images/vehicles/blue/vehicle2.png", true));
    /* 2 */ blueVehicleImages.push_back(std::make_unique<Image>(renderer, "assets/images/vehicles/blue/vehicle3.png", true));
    /* 3 */ blueVehicleImages.push_back(std::make_unique<Image>(renderer, "assets/images/vehicles/blue/vehicle4.png", true));
    /* 4 */ blueVehicleImages.push_back(std::make_unique<Image>(renderer, "assets/images/vehicles/blue/vehicle5.png", true));
    /* 5 */ blueVehicleImages.push_back(std::make_unique<Image>(renderer, "assets/images/vehicles/blue/vehicle6.png", true));
}
