// VroemVroem - City

#include "city.hpp"
#include <memory>
#include "fonts.hpp"
#include "font.hpp"
#include "rect.hpp"
#include "utils.hpp"
#include <cstring>

City::City(int id, const char *name, float x, float y, int population)
    : Object::Object(id, x, y), name(name), population(population) {}

const char *City::getName() const {
    return name;
}

int City::getPopulation() const {
    return population;
}

void City::setPopulation(int population) {
    this->population = population;
}

void City::draw(Canvas *canvas, const Camera *camera) const {
    std::unique_ptr<Rect> canvasRect = canvas->getRect();

    char cityLabel[128];
    sprintf(cityLabel, "%s (%d)", name, population);

    int tileSize = Camera::zoomLevels[camera->getZoom()];

    Font *textFont = Fonts::getInstance()->getTextFont();

    Rect cityRect;
    cityRect.height = tileSize / 2;
    cityRect.width = textFont->measure(cityLabel, cityRect.height);
    cityRect.x = static_cast<int>(x * tileSize - (camera->getX() * tileSize - canvasRect->width / 2)) - cityRect.width / 2;
    cityRect.y = static_cast<int>(y * tileSize - (camera->getY() * tileSize - canvasRect->height / 2)) - cityRect.height / 2;

    if (Rect::collision(canvasRect.get(), &cityRect) && camera->getZoom() >= 2) {
        // std::unique_ptr<Image> cityLabelImage = textFont->render(canvas, cityLabel, cityLabelRect.height, RGB(255, 255, 255));
        // cityLabelImage->draw(&cityLabelRect);
    }
}

const char *City::randomName(Random *random) {
    const char *consonants = "bcdfghjklmnpqrstvwxyz";
    size_t consonantsLength = strlen(consonants);
    const char *vowels = "aeiou";
    size_t vowelsLength = strlen(vowels);

    int nameLength = random->random(2, 5) * 2;

    char *randomName = new char[nameLength + 1]; // MEM LEAK!!!
    int pos = 0;
    while (pos < nameLength) {
        randomName[pos] = consonants[random->random(0, consonantsLength - 1)];
        randomName[pos + 1] = vowels[random->random(0, vowelsLength - 1)];
        pos += 2;
    }
    randomName[pos] = '\0';

    return randomName;
}
