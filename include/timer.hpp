// VroemVroem - Timer Header

#pragma once

#include <cstdint>

class Timer {
    public:
        static uint32_t callback(uint32_t interval, void *param);
};
