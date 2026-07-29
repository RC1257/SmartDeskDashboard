/**
 * @file main.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <SDL.h>

#include "hal/hal.h"
#include "ui.h"
#include "navigation.h"
#include "timer_controller.h"
#include "task_manager.h"
#include "quote_manager.h"
#include "settings_manager.h"
#include "theme_manager.h"
#include "clock_manager.h"
#include "startup_controller.h"

void startupFinished(lv_timer_t* timer)
{
    lv_screen_load_anim(
        ui_screenHome,
        LV_SCR_LOAD_ANIM_FADE_IN,
        500,
        0,
        false
    );

    Navigation::init();
    lv_timer_delete(timer);
}


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{

  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  sdl_hal_init(480, 320);

  /* Run the default demo */
  /* To try a different demo or example, replace this with one of: */
  /* - lv_demo_benchmark(); */
  /* - lv_demo_stress(); */
  /* - lv_example_label_1(); */
  /* - etc. */
  ui_init();

  // Remove default checkbox labels
  lv_checkbox_set_text(ui_Checkbox1, "");
  lv_checkbox_set_text(ui_Checkbox2, "");
  lv_checkbox_set_text(ui_Checkbox3, "");
  lv_checkbox_set_text(ui_Checkbox4, "");

  TimerController::init();
  TaskManager::init();
  QuoteManager::init();
  ThemeManager::init();
  SettingsManager::init();
  ClockManager::init();
  
  Navigation::init();
  StartupController::init();


  while(1) {
    Navigation::update();
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    uint32_t sleep_time_ms = lv_timer_handler();
    if(sleep_time_ms == LV_NO_TIMER_READY){
	sleep_time_ms =  LV_DEF_REFR_PERIOD;
    }
#ifdef _MSC_VER
    Sleep(sleep_time_ms);
#else
    usleep(sleep_time_ms * 1000);
#endif
  }

  return 0;
}


#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/
