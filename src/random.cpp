// VroemVroem - Random Object

#include "random.hpp"
#include <cstdint>
#include <cmath>

Random::Random(uint64_t seed) : seed(seed) {}

double Random::random() {
    double x = sin(seed++) * 10000;
    return x - floor(x);
}

int Random::randomInt(int min, int max) {
    return (int)(random() * (max - min + 1)) + min;
}
