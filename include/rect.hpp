// VroemVroem - Rect Header

#pragma once

struct Rect {
    int x;
    int y;
    int width;
    int height;

    Rect();

    Rect(int x, int y, int width, int height);

    bool containsPoint(int pointX, int pointY);

    bool collides(Rect *other);

    bool collides(Rect *other, float angle);
};
