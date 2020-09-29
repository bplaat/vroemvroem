// VroemVroem - Random Object Header

#pragma once

#include <cstdint>

class Random {
    public:
        uint64_t seed;

        Random(uint64_t seed);

        double random();

        int randomInt(int min, int max);
};
