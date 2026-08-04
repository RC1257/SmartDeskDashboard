#include <Arduino.h>
#include <lvgl.h>

#include "ui_update.h"

extern "C"
{
#include "ui.h"
}

static uint32_t lastLvglTick = 0;

void initUI()
{
    Serial.println("Initializing SquareLine UI...");

    lastLvglTick = millis();
    ui_init();

    Serial.println("SquareLine UI initialized.");
}

void updateUI()
{
    const uint32_t now = millis();
    const uint32_t elapsed = now - lastLvglTick;

    if (elapsed > 0)
    {
        lv_tick_inc(elapsed);
        lastLvglTick = now;
    }

    lv_timer_handler();
}