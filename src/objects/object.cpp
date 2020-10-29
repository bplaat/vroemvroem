// VroemVroem - Object

#include "objects/object.hpp"

namespace Objects {

Object::Object(int id, float x, float y)
    : id(id), x(x), y(y) {}

int Object::getId() const {
    return id;
}

float Object::getX() const {
    return x;
}

float Object::getY() const {
    return y;
}

void Object::update(float delta) {
    (void)delta;
}

}
