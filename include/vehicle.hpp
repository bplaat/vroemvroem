// VroemVroem - Vehicle Object Header

#pragma once

class Vehicle {
    public:
        int id;
        int type;
        float x;
        float y;
        float angle;

        Vehicle(int id, int type, float x, float y, float angle);
};
