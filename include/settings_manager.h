#pragma once

#include <stdint.h>

namespace SettingsManager
{
    void init();

    // Brightness
    void increaseBrightness();
    uint8_t getBrightness();

    // Volume
    void increaseVolume();
    uint8_t getVolume();
}