// VroemVroem - City Object

#include "city.hpp"
#include <memory>
#include <cstring>
#include "random.hpp"

City::City(int id, std::unique_ptr<char[]> name, int x, int y, int population)
    : id(id), name(std::move(name)), x(x), y(y), population(population) {}

std::unique_ptr<char[]> City::randomName(Random *random) {
    const char *consonants = "bcdfghjklmnpqrstvwxyz";
    size_t consonantsLength = strlen(consonants);
    const char *vowels = "aeiou";
    size_t vowelsLength = strlen(vowels);

    int nameLength = random->randomInt(2, 5) * 2;

    auto randomName = std::make_unique<char[]>(nameLength);
    int pos = 0;
    while (pos < nameLength) {
        randomName[pos] = consonants[random->randomInt(0, consonantsLength - 1)];
        randomName[pos + 1] = vowels[random->randomInt(0, vowelsLength - 1)];
        pos += 2;
    }
    randomName[pos] = '\0';

    return randomName;
}
