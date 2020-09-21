// VroemVroem - Utils Header

#pragma once

#include <iostream>

class Random {
    public:
        uint64_t seed;

        Random(uint64_t seed);

        double random();
};

char *file_read(const char *path);
