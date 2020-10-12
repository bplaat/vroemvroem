// VroemVroem - City Object Header

#pragma once

#include <memory>
#include "random.hpp"

class City {
    public:
        int id;
        std::unique_ptr<char[]> name;
        int x;
        int y;
        int population;

        City(int id, std::unique_ptr<char[]> name, int x, int y, int population);

        static std::unique_ptr<char[]> randomName(Random *random);
};
