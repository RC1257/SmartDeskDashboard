#include <Arduino.h>
#include <lvgl.h>

#include "screen_manager.h"
#include "state_manager.h"

extern "C"
{
#include "ui.h"
}

static void loadScreen(
    lv_obj_t *screenObject,
    Screen screenState,
    const char *screenName
)
{
    if (screenObject == nullptr)
    {
        Serial.print("Cannot load screen: ");
        Serial.print(screenName);
        Serial.println(" is null.");
        return;
    }

    setCurrentScreen(screenState);

    lv_screen_load_anim(
        screenObject,
        LV_SCR_LOAD_ANIM_FADE_IN,
        250,
        0,
        false
    );

    Serial.print("Switched to ");
    Serial.print(screenName);
    Serial.println(" screen.");
}

void goToSplashScreen()
{
    loadScreen(
        ui_ScreenStartup,
        Screen::Splash,
        "Splash"
    );
}

void goToHomeScreen()
{
    loadScreen(
        ui_screenHome,
        Screen::Home,
        "Home"
    );
}

void goToTimerScreen()
{
    loadScreen(
        ui_screenTimer,
        Screen::Timer,
        "Timer"
    );
}

void goToTasksScreen()
{
    loadScreen(
        ui_screenTasks,
        Screen::Tasks,
        "Tasks"
    );
}

void goToQuoteScreen()
{
    loadScreen(
        ui_screenQuote,
        Screen::Quote,
        "Quote"
    );
}

void goToSettingsScreen()
{
    loadScreen(
        ui_screenSettings,
        Screen::Settings,
        "Settings"
    );
}

void goToDisplayScreen()
{
    loadScreen(
        ui_screenDisplay,
        Screen::Display,
        "Display"
    );
}

void goToVolumeNetworkScreen()
{
    loadScreen(
        ui_screenVolNet,
        Screen::VolumeNetwork,
        "Volume and Network"
    );
}

void goToAboutScreen()
{
    loadScreen(
        ui_screenAbout,
        Screen::About,
        "About"
    );
}