#include "../include/settings_manager.h"
#include <Arduino.h>

#include "lvgl.h"
#include "ui.h"

namespace
{
    uint8_t brightness = 50;
    uint8_t volume = 50;

    void update_brightness_bar()
    {
        lv_bar_set_value(
            ui_BarBrightness,
            brightness,
            LV_ANIM_ON
        );
    }

    void update_volume_bar()
    {
        lv_bar_set_value(
            ui_BarBrightness_1,
            volume,
            LV_ANIM_ON
        );
    }
}

namespace SettingsManager
{
    void init()
    {
        brightness = 50;
        volume = 50;

        lv_bar_set_range(
            ui_BarBrightness,
            0,
            100
        );

        lv_bar_set_range(
            ui_BarBrightness_1,
            0,
            100
        );

        update_brightness_bar();
        update_volume_bar();
    }

    void increaseBrightness()
{
    brightness += 10;

    if(brightness > 100)
    {
        brightness = 10;
    }

    Serial.print("Brightness value: ");
    Serial.println(brightness);

    lv_bar_set_value(
        ui_BarBrightness,
        brightness,
        LV_ANIM_OFF
    );
}
    uint8_t getBrightness()
    {
        return brightness;
    }

    void increaseVolume()
{
    volume += 10;

    if(volume > 100)
    {
        volume = 10;
    }

    Serial.print("Volume value: ");
    Serial.println(volume);

    lv_bar_set_value(
        ui_BarBrightness_1,
        volume,
        LV_ANIM_OFF
    );
}

    uint8_t getVolume()
    {
        return volume;
    }
}