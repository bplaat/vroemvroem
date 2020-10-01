// VroemVroem - City Object Header

#pragma once

#include "random.hpp"

class City {
    public:
        int id;
        const char *name;
        int x;
        int y;
        int population;

        City(int id, const char *name, int x, int y, int population);

        ~City();

        static const char *randomName(Random *random);
};
