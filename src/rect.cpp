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
