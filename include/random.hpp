// VroemVroem - Random Header

#pragma once

#include <cstdint>

class Random {
    private:
        uint64_t seed;

    public:
        Random(uint64_t seed);

        uint64_t getSeed() const;

        void setSeed(uint64_t seed);

        double random();

        int random(int min, int max);
};
