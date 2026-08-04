#include "startup_controller.h"
#include "ui.h"
#include "lvgl.h"

#include "navigation.h"
#include "theme_manager.h"
#include "screen_manager.h"


namespace {

void startupFinished(lv_timer_t* timer)
{
       // Wait until the 500 ms transition is finished.
    goToHomeScreen();
    lv_timer_delete(timer);
}

}

void StartupController::init()
{
    lv_screen_load(ui_ScreenStartup);

    lv_timer_create(
        startupFinished,
        1800,
        nullptr
    );
}