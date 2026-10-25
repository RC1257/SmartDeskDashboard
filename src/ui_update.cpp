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

    // Remove SquareLine's default checkbox text
    lv_checkbox_set_text(ui_Checkbox1, "");
    lv_checkbox_set_text(ui_Checkbox2, "");
    lv_checkbox_set_text(ui_Checkbox3, "");
    lv_checkbox_set_text(ui_Checkbox4, "");

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