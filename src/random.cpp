// VroemVroem - Random

#include "random.hpp"
#include <cmath>
#include <ctime>

std::unique_ptr<Random> Random::instance = nullptr;

Random::Random(uint64_t seed) : seed(seed) {}

Random *Random::getInstance() {
    if (!instance) {
        instance = std::make_unique<Random>(time(nullptr));
    }
    return instance.get();
}

uint64_t Random::getSeed() const {
    return seed;
}

void Random::setSeed(uint64_t seed) {
    this->seed = seed;
}

double Random::random() {
    double x = sin(seed++) * 10000;
    return x - floor(x);
}

int Random::random(int min, int max) {
    return static_cast<int>(random() * (max - min + 1)) + min;
}
