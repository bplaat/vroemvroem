// VroemVroem - Random Header

#pragma once

#include <memory>

class Random {
    private:
        static std::unique_ptr<Random> instance;

        uint64_t seed;

    public:
        Random(uint64_t seed);

        static Random *getInstance();

        uint64_t getSeed() const;

        void setSeed(uint64_t seed);

        double random();

        int random(int min, int max);
};
