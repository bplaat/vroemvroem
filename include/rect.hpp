// VroemVroem - Rect Header

#pragma once

struct Rect {
    int x;
    int y;
    int width;
    int height;

    static bool collision(Rect *a, Rect *b);
};
