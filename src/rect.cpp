// VroemVroem - Rect

#include "rect.hpp"

Rect::Rect()
    : x(0), y(0), width(0), height(0) {}

Rect::Rect(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

bool Rect::containsPoint(int pointX, int pointY) {
    return pointX >= x && pointY >= y &&
        pointX < x + width && pointY < y + height;
}

bool Rect::collides(Rect *other) {
    return x < other->x + other->width &&
        x + width > other->x &&
        y < other->y + other->height &&
        y + height > other->y;
}

bool Rect::collides(Rect *other, float angle) { // TODO
    //  int x0 = (other->x - other->x + other->width / 2);
    //  new_x_point = old_x_point * cos(Angle) - old_y_point * sin(Angle);
    //  new_y_point = old_y_point * cos(Angle) + old_x_point * sin(Angle);

    return x < other->x + other->width &&
        x + width > other->x &&
        y < other->y + other->height &&
        y + height > other->y;
}
