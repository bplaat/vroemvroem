// VroemVroem - City Object

#include "city.hpp"
#include "random.hpp"
#include <cstring>

City::City(int id, const char *name, int x, int y, int population)
    : id(id), name(name), x(x), y(y), population(population) {}

City::~City() {
    delete name;
}

const char *City::randomName(Random *random) {
    const char *consonants = "bcdfghjklmnpqrstvwxyz";
    int consonantsLength = strlen(consonants);

    const char *vowels = "aeiou";
    int vowelsLength = strlen(vowels);

    int length = random->randomInt(2, 5) * 2;

    char *randomName = new char[length * 2 + 1];
    int i = 0;
    while (i < length) {
        randomName[i] = consonants[random->randomInt(0, consonantsLength - 1)];
        randomName[i + 1] = vowels[random->randomInt(0, vowelsLength - 1)];
        i += 2;
    }
    randomName[i] = 0;

    return randomName;
}
