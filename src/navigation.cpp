#include "navigation.h"
#include "timer_controller.h"
#include "../include/task_manager.h"
#include "../include/settings_manager.h"
#include "../include/theme_manager.h"
#include "../include/clock_manager.h"
#include "navigation_keys.h"

#include "lvgl.h"
#include "ui.h"

#include <stdio.h>
#include <Arduino.h>

namespace
{
    lv_group_t* navigation_group = nullptr;
    lv_obj_t* startup_screen = nullptr;

    // Forward declarations
    void setup_startup_screen();
    void setup_home_screen();
    void setup_timer_screen();
    void setup_tasks_screen();
    void setup_quote_screen();
    void setup_settings_screen();
    void setup_display_screen();
    void setup_volnet_screen();
    void setup_about_screen();
    void setup_destination_screen(lv_obj_t* screen);

    void startup_key_event(lv_event_t* event);
    void home_key_event(lv_event_t* event);
    void home_screen_loaded_event(lv_event_t* event);
    void timer_key_event(lv_event_t* event);
    void tasks_key_event(lv_event_t* event);
    void quote_key_event(lv_event_t* event);
    void main_settings_key_event(lv_event_t* event);
    void display_key_event(lv_event_t* event);
    void volnet_key_event(lv_event_t* event);
    void about_key_event(lv_event_t* event);
    void theme_screen_loaded_event(lv_event_t* event);
    void apply_theme_async(void* user_data);
    
    /**
     * Makes an object selectable and displays an outline
     * while the object is focused.
     */

    void prepare_focusable_object(lv_obj_t* object)
    {
        if(object == nullptr)
        {
            return;
        }

        lv_obj_add_flag(object, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(object, LV_OBJ_FLAG_CLICK_FOCUSABLE);

        lv_obj_set_style_outline_width(
            object,
            3,
            LV_PART_MAIN | LV_STATE_FOCUSED
        );

        lv_obj_set_style_outline_pad(
            object,
            3,
            LV_PART_MAIN | LV_STATE_FOCUSED
        );

    
        lv_obj_set_style_outline_opa(
            object,
            LV_OPA_COVER,
            LV_PART_MAIN | LV_STATE_FOCUSED
        );
    }

    /**
     * Removes every object from the navigation group.
     */

    void style_checkbox(lv_obj_t* checkbox)
{
    const lv_color_t gray_outline = lv_color_hex(0x808080);
    const lv_color_t light_gray_fill = lv_color_hex(0xF7F7F7);
    const lv_color_t dark_gray_check = lv_color_hex(0x555555);

    /*
     * Unchecked state:
     * white/transparent interior with a gray outline.
     */
    lv_obj_set_style_bg_opa(
        checkbox,
        LV_OPA_TRANSP,
        LV_PART_INDICATOR | LV_STATE_DEFAULT
    );

    lv_obj_set_style_border_color(
        checkbox,
        gray_outline,
        LV_PART_INDICATOR | LV_STATE_DEFAULT
    );

    lv_obj_set_style_border_width(
        checkbox,
        2,
        LV_PART_INDICATOR | LV_STATE_DEFAULT
    );

    /*
     * Checked state:
     * light-gray fill, gray outline, and dark-gray checkmark.
     */
    lv_obj_set_style_bg_color(
        checkbox,
        light_gray_fill,
        LV_PART_INDICATOR | LV_STATE_CHECKED
    );

    lv_obj_set_style_bg_opa(
        checkbox,
        LV_OPA_COVER,
        LV_PART_INDICATOR | LV_STATE_CHECKED
    );

    lv_obj_set_style_bg_grad_dir(
        checkbox,
        LV_GRAD_DIR_NONE,
        LV_PART_INDICATOR | LV_STATE_CHECKED
    );

    lv_obj_set_style_border_color(
        checkbox,
        gray_outline,
        LV_PART_INDICATOR | LV_STATE_CHECKED
    );

    lv_obj_set_style_border_width(
        checkbox,
        2,
        LV_PART_INDICATOR | LV_STATE_CHECKED
    );

    lv_obj_set_style_text_color(
        checkbox,
        dark_gray_check,
        LV_PART_INDICATOR | LV_STATE_CHECKED
    );
}

    void clear_navigation_group()
    {
        if(navigation_group != nullptr)
        {
            lv_group_remove_all_objs(navigation_group);
        }
    }

    /**
     * Prevents the current key press from carrying over
     * to the next screen.
     */
    void wait_for_key_release()
    {
        lv_indev_t* indev = lv_indev_active();

        if(indev != nullptr)
        {
            lv_indev_wait_release(indev);
        }
    }

    /**
     * Configures navigation for the Startup screen.
     *
     * The startup screen itself receives the Enter key.
     */
    void setup_startup_screen()
    {
        if(navigation_group == nullptr || startup_screen == nullptr)
        {
            return;
        }

        clear_navigation_group();

        lv_obj_add_flag(startup_screen, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(startup_screen, LV_OBJ_FLAG_CLICK_FOCUSABLE);

        lv_group_add_obj(navigation_group, startup_screen);
        lv_group_focus_obj(startup_screen);
    }

    /**
     * Configures navigation for the four Home-screen panels.
     */
    void setup_home_screen()
    {
        if(navigation_group == nullptr)
        {
            return;
        }

        clear_navigation_group();

        prepare_focusable_object(ui_panelTimer);
        prepare_focusable_object(ui_panelTasks);
        prepare_focusable_object(ui_panelQuote);
        prepare_focusable_object(ui_panelSettings);

        lv_group_add_obj(navigation_group, ui_panelTimer);
        lv_group_add_obj(navigation_group, ui_panelTasks);
        lv_group_add_obj(navigation_group, ui_panelQuote);
        lv_group_add_obj(navigation_group, ui_panelSettings);

        lv_group_focus_obj(ui_panelTimer);
    }

    void setup_timer_screen()
    {
        if(navigation_group == nullptr)
        {
            return;
        }

        clear_navigation_group();

        prepare_focusable_object(ui_PanelStartPause);
        prepare_focusable_object(ui_PanelAdjustTimer);
        prepare_focusable_object(ui_PanelReturnHome);

        lv_group_add_obj(navigation_group, ui_PanelStartPause);
        lv_group_add_obj(navigation_group, ui_PanelAdjustTimer);
        lv_group_add_obj(navigation_group, ui_PanelReturnHome);

        lv_group_focus_obj(ui_PanelStartPause);
    }

    void setup_tasks_screen()
    {
        if(navigation_group == nullptr)
        {
            return;
        }

        clear_navigation_group();

        prepare_focusable_object(ui_PanelToDo1);
        prepare_focusable_object(ui_PanelToDo2);
        prepare_focusable_object(ui_PanelToDo3);
        prepare_focusable_object(ui_PanelToDo4);

        lv_group_add_obj(navigation_group, ui_PanelToDo1);
        lv_group_add_obj(navigation_group, ui_PanelToDo2);
        lv_group_add_obj(navigation_group, ui_PanelToDo3);
        lv_group_add_obj(navigation_group, ui_PanelToDo4);

        lv_group_focus_obj(ui_PanelToDo1);
        
        style_checkbox(ui_Checkbox1);
        style_checkbox(ui_Checkbox2);
        style_checkbox(ui_Checkbox3);   
        style_checkbox(ui_Checkbox4);

    }




    




    void setup_quote_screen()
    {
    clear_navigation_group();

    prepare_focusable_object(ui_screenQuote);

    lv_group_add_obj(navigation_group, ui_screenQuote);
    lv_group_focus_obj(ui_screenQuote);
    }

    void setup_settings_screen()
{
    clear_navigation_group();

    lv_group_set_editing(navigation_group, false);

    prepare_focusable_object(ui_PanelDisplay);
    prepare_focusable_object(ui_PanelVolNet);
    prepare_focusable_object(ui_PanelAbout);

    lv_group_add_obj(navigation_group, ui_PanelDisplay);
    lv_group_add_obj(navigation_group, ui_PanelVolNet);
    lv_group_add_obj(navigation_group, ui_PanelAbout);

    lv_group_focus_obj(ui_PanelDisplay);
}

void setup_display_screen()
{
    if(navigation_group == nullptr)
    {
        return;
    }

    clear_navigation_group();

    lv_group_set_editing(navigation_group, true);


    prepare_focusable_object(ui_PanelLight);
    prepare_focusable_object(ui_PanelDark);
    prepare_focusable_object(ui_PanelPink);

    lv_obj_add_flag(ui_PanelLight, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(ui_PanelDark, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(ui_PanelPink, LV_OBJ_FLAG_CHECKABLE);

    lv_group_add_obj(navigation_group, ui_PanelLight);
    lv_group_add_obj(navigation_group, ui_PanelDark);
    lv_group_add_obj(navigation_group, ui_PanelPink);

    /*
     * Restore focus to the currently selected theme
     * instead of always returning to Light.
     */
    switch(ThemeManager::getTheme())
    {
        case ThemeManager::Theme::LIGHT:
            lv_group_focus_obj(ui_PanelLight);
            break;

        case ThemeManager::Theme::DARK:
            lv_group_focus_obj(ui_PanelDark);
            break;

        case ThemeManager::Theme::PINK:
            lv_group_focus_obj(ui_PanelPink);
            break;
    }
}
void setup_volnet_screen()
{
    if(navigation_group == nullptr)
    {
        return;
    }

    clear_navigation_group();

    lv_group_set_editing(navigation_group, true);

    prepare_focusable_object(ui_PanelConnection);
    prepare_focusable_object(ui_PanelTimeSync);

    lv_group_add_obj(navigation_group, ui_PanelConnection);
    lv_group_add_obj(navigation_group, ui_PanelTimeSync);

    lv_group_focus_obj(ui_PanelConnection);
}

void setup_about_screen()
{
    if(navigation_group == nullptr)
    {
        return;
    }

    clear_navigation_group();

    prepare_focusable_object(ui_screenAbout);

    lv_group_add_obj(navigation_group, ui_screenAbout);
    lv_group_focus_obj(ui_screenAbout);
}







    /**
     * Temporary navigation setup for destination screens.
     *
     * Later, each destination screen will receive its own
     * setup function containing its actual buttons and controls.
     */
    void setup_destination_screen(lv_obj_t* screen)
    {
        if(navigation_group == nullptr || screen == nullptr)
        {
            return;
        }

        clear_navigation_group();

        lv_obj_add_flag(screen, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(screen, LV_OBJ_FLAG_CLICK_FOCUSABLE);

        lv_group_add_obj(navigation_group, screen);
        lv_group_focus_obj(screen);
    }

    /**
     * Handles Enter on the Startup screen.
     */
    void startup_key_event(lv_event_t* event)
    {
        if(lv_event_get_code(event) != LV_EVENT_KEY)
        {
            return;
        }

        const uint32_t key = lv_event_get_key(event);

        if(key == LV_KEY_ENTER)
        {
            wait_for_key_release();

            lv_screen_load(ui_screenHome);
            setup_home_screen();
        }
    }

    /**
     * Handles navigation on the Home screen.
     *
     * Layout:
     *
     * Timer       Tasks
     * Quote       Settings
     */
    void home_key_event(lv_event_t* event)
    {
        if(lv_event_get_code(event) != LV_EVENT_KEY)
        {
            return;
        }

        lv_obj_t* focused_object = lv_event_get_target_obj(event);
        const uint32_t key = lv_event_get_key(event);

        /*
         * Timer panel
         */
        if(focused_object == ui_panelTimer)
        {
            if(key == LV_KEY_RIGHT)
            {
                lv_group_focus_obj(ui_panelTasks);
            }
            else if(key == LV_KEY_LEFT)
            {
                lv_group_focus_obj(ui_panelTasks);
            }
            else if(key == LV_KEY_DOWN)
            {
                lv_group_focus_obj(ui_panelQuote);
            }
            else if(key == LV_KEY_UP)
            {
                lv_group_focus_obj(ui_panelQuote);
            }
            else if(key == LV_KEY_ENTER)
            {
                wait_for_key_release();

                lv_screen_load(ui_screenTimer);
                setup_timer_screen();
            }

            return;
        }

        /*
         * Tasks panel
         */
        if(focused_object == ui_panelTasks)
        {
            if(key == LV_KEY_LEFT)
            {
                lv_group_focus_obj(ui_panelTimer);
            }
            else if(key == LV_KEY_RIGHT)
            {
                lv_group_focus_obj(ui_panelTimer);
            }
            else if(key == LV_KEY_DOWN)
            {
                lv_group_focus_obj(ui_panelSettings);
            }
            else if(key == LV_KEY_UP)
            {
                lv_group_focus_obj(ui_panelSettings);
            }
            else if(key == LV_KEY_ENTER)
            {
                wait_for_key_release();

                lv_screen_load(ui_screenTasks);
                setup_tasks_screen();
            }

            return;
        }

        /*
         * Quote panel
         */
        if(focused_object == ui_panelQuote)
        {
            if(key == LV_KEY_RIGHT)
            {
                lv_group_focus_obj(ui_panelSettings);
            }
            else if(key == LV_KEY_LEFT)
            {
                lv_group_focus_obj(ui_panelSettings);
            }
            else if(key == LV_KEY_UP)
            {
                lv_group_focus_obj(ui_panelTimer);
            }
            else if(key == LV_KEY_DOWN)
            {
                lv_group_focus_obj(ui_panelTimer);
            }
            else if(key == LV_KEY_ENTER)
            {
                wait_for_key_release();

                lv_screen_load(ui_screenQuote);
                setup_quote_screen();
            }

            return;
        }

        /*
         * Settings panel
         */
        if(focused_object == ui_panelSettings)
        {
            if(key == LV_KEY_LEFT)
            {
                lv_group_focus_obj(ui_panelQuote);
            }
            else if(key == LV_KEY_RIGHT)
            {
                lv_group_focus_obj(ui_panelQuote);
            }
            else if(key == LV_KEY_UP)
            {
                lv_group_focus_obj(ui_panelTasks);
            }
            else if(key == LV_KEY_DOWN)
            {
                lv_group_focus_obj(ui_panelTasks);
            }
            else if(key == LV_KEY_ENTER)
            {
                wait_for_key_release();

                lv_screen_load(ui_screenSettings);
                setup_settings_screen();
            }

            return;
        }
    }

    void timer_key_event(lv_event_t* event)
{
    if(lv_event_get_code(event) != LV_EVENT_KEY)
    {
        return;
    }

    lv_obj_t* focused_object = lv_event_get_target_obj(event);
    const uint32_t key = lv_event_get_key(event);

    /*
     * ==================================================
     * ADJUST MODE
     * ==================================================
     *
     * UP   = next duration
     * DOWN = confirm duration
     *
     * LEFT/RIGHT do nothing while adjusting.
     */
    if(TimerController::isAdjusting())
    {
        if(key == LV_KEY_UP)
        {
            TimerController::nextDuration();
        }
        else if(key == LV_KEY_DOWN)
        {
            TimerController::confirmDuration();

            lv_group_focus_obj(ui_PanelAdjustTimer);
        }

        return;
    }

    /*
     * ==================================================
     * START / PAUSE PANEL
     * ==================================================
     */
    if(focused_object == ui_PanelStartPause)
    {
        if(key == LV_KEY_RIGHT)
        {
            lv_group_focus_obj(ui_PanelAdjustTimer);
        }
        else if(key == LV_KEY_LEFT)
        {
            lv_group_focus_obj(ui_PanelReturnHome);
        }
        else if(key == LV_KEY_ENTER)
        {
            TimerController::toggle();
        }

        return;
    }

    /*
     * ==================================================
     * ADJUST TIMER PANEL
     * ==================================================
     */
    if(focused_object == ui_PanelAdjustTimer)
    {
        if(key == LV_KEY_RIGHT)
        {
            lv_group_focus_obj(ui_PanelReturnHome);
        }
        else if(key == LV_KEY_LEFT)
        {
            lv_group_focus_obj(ui_PanelStartPause);
        }
        else if(key == LV_KEY_ENTER)
        {
            if(!TimerController::isRunning())
            {
                TimerController::beginAdjust();
            }
        }

        return;
    }

    /*
     * ==================================================
     * RETURN HOME PANEL
     * ==================================================
     */
    if(focused_object == ui_PanelReturnHome)
    {
        if(key == LV_KEY_RIGHT)
        {
            lv_group_focus_obj(ui_PanelStartPause);
        }
        else if(key == LV_KEY_LEFT)
        {
            lv_group_focus_obj(ui_PanelAdjustTimer);
        }
        else if(key == LV_KEY_ENTER)
        {
            wait_for_key_release();

            lv_screen_load(ui_screenHome);
            setup_home_screen();
        }

        return;
    }
}

void tasks_key_event(lv_event_t* event)
{
    if(lv_event_get_code(event) != LV_EVENT_KEY)
    {
        return;
    }

    lv_obj_t* focused_object = lv_event_get_target_obj(event);
    const uint32_t key = lv_event_get_key(event);
    printf("KEY RECEIVED: %lu\n", static_cast<unsigned long>(key));

    /*
     * LEFT returns to Home from anywhere on the Tasks screen.
     */
    if(key == LV_KEY_LEFT)
    {
        wait_for_key_release();

        lv_screen_load(ui_screenHome);
        setup_home_screen();
        return;
    }

    /*
     * Task 1
     */
    if(focused_object == ui_PanelToDo1)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelToDo2);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelToDo4);
        }
        else if(key == LV_KEY_ENTER)
        {
            TaskManager::toggleTask(0);
        }

        return;
    }

    /*
     * Task 2
     */
    if(focused_object == ui_PanelToDo2)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelToDo3);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelToDo1);
        }
        else if(key == LV_KEY_ENTER)
        {
            TaskManager::toggleTask(1);
        }

        return;
    }

    /*
     * Task 3
     */
    if(focused_object == ui_PanelToDo3)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelToDo4);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelToDo2);
        }
        else if(key == LV_KEY_ENTER)
        {
            TaskManager::toggleTask(2);
        }

        return;
    }

    /*
     * Task 4
     */
    if(focused_object == ui_PanelToDo4)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelToDo1);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelToDo3);
        }
        else if(key == LV_KEY_ENTER)
        {
            TaskManager::toggleTask(3);
        }

        return;
    }
}

void quote_key_event(lv_event_t* event)
{
    if(lv_event_get_code(event) != LV_EVENT_KEY)
    {
        return;
    }

    const uint32_t key = lv_event_get_key(event);

    if(key == LV_KEY_LEFT)
    {
        wait_for_key_release();

        lv_screen_load(ui_screenHome);
        setup_home_screen();
    }
}

void apply_theme_async(void* user_data)
{
    (void)user_data;

    ThemeManager::applyCurrentTheme();
}

    /**
     * Restores Home-screen navigation whenever Home is loaded.
     *
     * This is useful when a SquareLine Back button returns to Home.
     */
    void home_screen_loaded_event(lv_event_t* event)
{
    (void)event;

    setup_home_screen();

    lv_async_call(
        apply_theme_async,
        nullptr
    );
}
    void theme_screen_loaded_event(lv_event_t* event)
{
    (void)event;

    lv_async_call(
        apply_theme_async,
        nullptr
    );
}


void main_settings_key_event(lv_event_t* event)
{
    if(lv_event_get_code(event) != LV_EVENT_KEY)
    {
        return;
    }

    lv_obj_t* focused_object = lv_event_get_target_obj(event);
    const uint32_t key = lv_event_get_key(event);

    /*
     * LEFT returns to the Home screen.
     */
    if(key == LV_KEY_LEFT)
    {
        wait_for_key_release();

        lv_screen_load(ui_screenHome);
        setup_home_screen();
        return;
    }

    /*
     * Display
     */
    if(focused_object == ui_PanelDisplay)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelVolNet);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelAbout);
        }
        else if(key == LV_KEY_ENTER)
        {
            wait_for_key_release();

            lv_screen_load(ui_screenDisplay);

            /*
             * Temporary setup until the Display navigation
             * function is added.
             */
            setup_display_screen();
        }

        return;
    }

    /*
     * Volume & Network
     */
    if(focused_object == ui_PanelVolNet)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelAbout);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelDisplay);
        }
        else if(key == LV_KEY_ENTER)
        {
            wait_for_key_release();

            lv_screen_load(ui_screenVolNet);

            /*
             * Temporary setup until the Volume & Network
             * navigation function is added.
             */
            setup_volnet_screen();
        }

        return;
    }

    /*
     * About
     */
    if(focused_object == ui_PanelAbout)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelDisplay);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelVolNet);
        }
        else if(key == LV_KEY_ENTER)
        {
            wait_for_key_release();

            lv_screen_load(ui_screenAbout);

            /*
             * Temporary setup until the About navigation
             * function is added.
             */
            setup_about_screen();
        }

        return;
    }
}  

void display_key_event(lv_event_t* event)
{
    if(lv_event_get_code(event) != LV_EVENT_KEY)
    {
        return;
    }

    lv_obj_t* focused_object = lv_event_get_target_obj(event);
    const uint32_t key = lv_event_get_key(event);

    Serial.print("Display screen key: ");
    Serial.println(static_cast<unsigned long>(key));

    /*
     * LEFT returns to Main Settings.
     */
    if(key == LV_KEY_LEFT)
    {
        wait_for_key_release();

        lv_screen_load(ui_screenSettings);
        setup_settings_screen();
        return;
    }

    /*
     * RIGHT increases brightness.
     */
    if(key == LV_KEY_RIGHT)
    {
    Serial.println("Increasing brightness");

    SettingsManager::increaseBrightness();

    lv_event_stop_processing(event);

    return;
}
    /*
     * LIGHT THEME
     */
    if(focused_object == ui_PanelLight)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelDark);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelPink);
        }
        else if(key == LV_KEY_ENTER)
        {
            ThemeManager::setTheme(
                ThemeManager::Theme::LIGHT
            );
        }

        return;
    }

    /*
     * DARK THEME
     */
    if(focused_object == ui_PanelDark)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelPink);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelLight);
        }
        else if(key == LV_KEY_ENTER)
        {
            ThemeManager::setTheme(
                ThemeManager::Theme::DARK
            );
        }

        return;
    }

    /*
     * PINK THEME
     */
    if(focused_object == ui_PanelPink)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelLight);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelDark);
        }
        else if(key == LV_KEY_ENTER)
        {
            ThemeManager::setTheme(
                ThemeManager::Theme::PINK
            );
        }

        return;
    }
}

void volnet_key_event(lv_event_t* event)
{
    if(lv_event_get_code(event) != LV_EVENT_KEY)
    {
        return;
    }

    lv_obj_t* focused_object = lv_event_get_target_obj(event);
    const uint32_t key = lv_event_get_key(event);

    Serial.print("Volume screen key: ");
    Serial.println(static_cast<unsigned long>(key));

    /*
     * LEFT returns to Settings.
     */
    if(key == LV_KEY_LEFT)
    {
        wait_for_key_release();

        lv_screen_load(ui_screenSettings);
        setup_settings_screen();
        return;
    }

    /*
     * RIGHT increases volume.
     */
    if(key == LV_KEY_RIGHT)
    {
    Serial.println("Increasing volume");

    SettingsManager::increaseVolume();

    lv_event_stop_processing(event);
    return;
    }
    /*
     * Connection
     */
    if(focused_object == ui_PanelConnection)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelTimeSync);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelTimeSync);
        }
        else if(key == LV_KEY_ENTER)
        {
            /*
             * Future:
             * Open Bluetooth/WiFi settings.
             */
        }

        return;
    }

    /*
     * Time Sync
     */
    if(focused_object == ui_PanelTimeSync)
    {
        if(key == LV_KEY_DOWN)
        {
            lv_group_focus_obj(ui_PanelConnection);
        }
        else if(key == LV_KEY_UP)
        {
            lv_group_focus_obj(ui_PanelConnection);
        }
        else if(key == LV_KEY_ENTER)
        {
            /*
             * Future:
             * Synchronize clock.
             */
        }

        return;
    }
}

void about_key_event(lv_event_t* event)
{
    if(lv_event_get_code(event) != LV_EVENT_KEY)
    {
        return;
    }

    const uint32_t key = lv_event_get_key(event);

    if(key == LV_KEY_LEFT)
    {
        wait_for_key_release();

        lv_screen_load(ui_screenSettings);
        setup_settings_screen();
    }
}

}


namespace Navigation
{

    void init()
    {
        /*
         * The SDL simulator should already have a default LVGL group
         * connected to the keyboard.
         */
        navigation_group = lv_group_get_default();

        if(navigation_group == nullptr)
        {
            navigation_group = lv_group_create();
            lv_group_set_default(navigation_group);
        }

        /*
         * ui_init() should already have loaded the Startup screen.
         */
        startup_screen = lv_screen_active();

        /*
         * Startup-screen keyboard event.
         */
        lv_obj_add_event_cb(
            startup_screen,
            startup_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        /*
         * Home-screen keyboard events.
         *
         * There are intentionally no LV_EVENT_CLICKED callbacks here.
         * Enter is handled only through home_key_event().
         */
        lv_obj_add_event_cb(
            ui_panelTimer,
            home_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_panelTasks,
            home_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_panelQuote,
            home_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_panelSettings,
            home_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        /*
         * Restore navigation whenever the Home screen is loaded.
         */
        lv_obj_add_event_cb(
            ui_screenHome,
            home_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelStartPause,
            timer_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelAdjustTimer,
            timer_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelReturnHome,
            timer_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelToDo1,
            tasks_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelToDo2,
            tasks_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelToDo3,
            tasks_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelToDo4,
            tasks_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenQuote,
            quote_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelDisplay,
            main_settings_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelVolNet,
            main_settings_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelAbout,
            main_settings_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelLight,
            display_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelDark,
            display_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelPink,
            display_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelConnection,
            volnet_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_PanelTimeSync,
            volnet_key_event,
            LV_EVENT_KEY,
            nullptr
        );
        lv_obj_add_event_cb(
            ui_screenAbout,
            about_key_event,
            LV_EVENT_KEY,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenTimer,
            theme_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenTasks,
            theme_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenQuote,
            theme_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenSettings,
            theme_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenDisplay,
            theme_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenVolNet,
            theme_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );

        lv_obj_add_event_cb(
            ui_screenAbout,
            theme_screen_loaded_event,
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );



        /*
         * Begin on the Startup screen.
         */
        setup_startup_screen();
    }

    void update()
    {
        /*
         * Navigation is event-driven, so no repeated work is
         * currently required here.
         */
    }
}
