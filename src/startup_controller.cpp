#include "startup_controller.h"
#include "ui.h"
#include "lvgl.h"

#include "navigation.h"
#include "theme_manager.h"


namespace {

void startupFinished(lv_timer_t* timer)
{
    lv_screen_load_anim(
        ui_screenHome,
        LV_SCR_LOAD_ANIM_FADE_IN,
        500,
        0,
        false
    );

    // Wait until the 500 ms transition is finished.

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