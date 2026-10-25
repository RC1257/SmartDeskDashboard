#include <Arduino.h>
#include <lvgl.h>

#include "navigation_input.h"
#include "settings_manager.h"

extern "C"
{
#include "ui.h"
}

// Navigation-module wiring
namespace
{
    constexpr uint8_t PIN_MID   = 8;
    constexpr uint8_t PIN_UP    = 15;
    constexpr uint8_t PIN_DOWN  = 16;
    constexpr uint8_t PIN_LEFT  = 17;
    constexpr uint8_t PIN_RIGHT = 18;

    constexpr uint32_t DEBOUNCE_MS = 35;

    lv_indev_t* keypadInput = nullptr;

    uint32_t lastKey = 0;
    bool lastRawPressed = false;
    bool stablePressed = false;
    bool rightActionConsumed = false;
    uint32_t lastChangeTime = 0;

    uint32_t readRawKey()
    {
        if(digitalRead(PIN_UP) == LOW)
        {
            return LV_KEY_UP;
        }

        if(digitalRead(PIN_DOWN) == LOW)
        {
            return LV_KEY_DOWN;
        }

        if(digitalRead(PIN_LEFT) == LOW)
        {
            return LV_KEY_LEFT;
        }

        if(digitalRead(PIN_RIGHT) == LOW)
        {
            return LV_KEY_RIGHT;
        }

        if(digitalRead(PIN_MID) == LOW)
        {
            return LV_KEY_ENTER;
        }

        return 0;
    }

    const char* keyName(uint32_t key)
    {
        switch(key)
        {
            case LV_KEY_UP:
                return "UP";

            case LV_KEY_DOWN:
                return "DOWN";

            case LV_KEY_LEFT:
                return "LEFT";


            case LV_KEY_RIGHT:
                return "RIGHT";

            case LV_KEY_ENTER:
                return "CENTER";

            default:
                return "UNKNOWN";

        }
    }

    void readNavigationInput(
        lv_indev_t* inputDevice,
        lv_indev_data_t* data
    )
    {
        (void)inputDevice;

        const uint32_t rawKey = readRawKey();
        const bool rawPressed = rawKey != 0;
        const uint32_t now = millis();

        /*
         * Restart debounce timing whenever the raw input changes.
         */
        if(
            rawPressed != lastRawPressed ||
            (rawPressed && rawKey != lastKey)
        )
        {
            lastRawPressed = rawPressed;
            lastChangeTime = now;

            if(rawPressed)
            {
                lastKey = rawKey;
            }
        }

        /*
         * Accept the change only after it remains stable.
         */
        if(now - lastChangeTime >= DEBOUNCE_MS)
        {
            if(stablePressed != rawPressed)
            {
                stablePressed = rawPressed;

                if(stablePressed)
{
    Serial.print("Navigation pressed: ");
    Serial.println(keyName(lastKey));

    rightActionConsumed = false;

    if(lastKey == LV_KEY_RIGHT)
    {
        lv_obj_t* activeScreen = lv_screen_active();

        if(activeScreen == ui_screenDisplay)
        {
            Serial.println("Increasing brightness directly");
            SettingsManager::increaseBrightness();
            rightActionConsumed = true;
        }
        else if(activeScreen == ui_screenVolNet)
        {
            Serial.println("Increasing volume directly");
            SettingsManager::increaseVolume();
            rightActionConsumed = true;
        }
    }
}
else
{
    rightActionConsumed = false;
}
            }
        }

        if(stablePressed && !rightActionConsumed)
{
    data->state = LV_INDEV_STATE_PRESSED;
    data->key = lastKey;
}
else
{
    data->state = LV_INDEV_STATE_RELEASED;
    data->key = lastKey;
}
    }
}

namespace NavigationInput
{
    void init()
    {
        Serial.println("Initializing physical navigation input...");

        /*
         * COM is connected to GND, so each signal uses the
         * ESP32's internal pull-up resistor. Pressed = LOW.
         */
        pinMode(PIN_UP, INPUT_PULLUP);
        pinMode(PIN_DOWN, INPUT_PULLUP);
        pinMode(PIN_LEFT, INPUT_PULLUP);
        pinMode(PIN_RIGHT, INPUT_PULLUP);
        pinMode(PIN_MID, INPUT_PULLUP);

        keypadInput = lv_indev_create();

        if(keypadInput == nullptr)
        {
            Serial.println(
                "ERROR: Could not create LVGL keypad input device."
            );
            return;
        }

        lv_indev_set_type(
            keypadInput,
            LV_INDEV_TYPE_KEYPAD
        );

        lv_indev_set_read_cb(
            keypadInput,
            readNavigationInput
        );

        lv_group_t* navigationGroup =
            lv_group_get_default();

        if(navigationGroup == nullptr)
        {
            Serial.println(
                "ERROR: Default LVGL navigation group does not exist."
            );
            return;
        }

        lv_indev_set_group(
            keypadInput,
            navigationGroup
        );

        Serial.println("Physical navigation input initialized.");
    }
}