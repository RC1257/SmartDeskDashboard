#include "timer_controller.h"

#include "lvgl.h"
#include "ui.h"

#include <stdio.h>

namespace
{
    /*
     * Available timer durations in seconds:
     *
     * 00:30
     * 01:00
     * 05:00
     * 10:00
     * 15:00
     * 20:00
     * 25:00
     * 30:00
     */
    constexpr uint32_t duration_options[] =
    {
        30,
        60,
        300,
        600,
        900,
        1200,
        1500,
        1800
    };

    constexpr uint32_t duration_count =
        sizeof(duration_options) / sizeof(duration_options[0]);

    /*
     * Start with 30 seconds selected.
     */
    uint32_t duration_index = 0;

    uint32_t selected_duration_seconds =
        duration_options[duration_index];

    uint32_t remaining_seconds =
        selected_duration_seconds;

    TimerController::State timer_state =
        TimerController::State::STOPPED;

    /*
     * LVGL timer used for the one-second countdown.
     */
    lv_timer_t* countdown_timer = nullptr;


    /*
     * --------------------------------------------------
     * DISPLAY HELPERS
     * --------------------------------------------------
     */

    void update_time_label()
    {
        const uint32_t minutes = remaining_seconds / 60;
        const uint32_t seconds = remaining_seconds % 60;

        char buffer[16];

        snprintf(
            buffer,
            sizeof(buffer),
            "%02lu:%02lu",
            static_cast<unsigned long>(minutes),
            static_cast<unsigned long>(seconds)
        );

        lv_label_set_text(
            ui_LabelTimerCountdown,
            buffer
        );
    }


    void update_start_pause_label()
    {
        switch(timer_state)
        {
            case TimerController::State::RUNNING:
                lv_label_set_text(
                    ui_LabelStartPause,
                    "Pause"
                );
                break;

            case TimerController::State::PAUSED:
                lv_label_set_text(
                    ui_LabelStartPause,
                    "Resume"
                );
                break;

            case TimerController::State::STOPPED:
            case TimerController::State::ADJUSTING:
            default:
                lv_label_set_text(
                    ui_LabelStartPause,
                    "Start"
                );
                break;
        }
    }


    void update_arc()
    {
        /*
         * Use a 0-100 percentage for the arc.
         */
        lv_arc_set_range(
            ui_ArcTimer,
            0,
            100
        );

        if(selected_duration_seconds == 0)
        {
            lv_arc_set_value(
                ui_ArcTimer,
                0
            );

            return;
        }

        uint32_t percentage =
            (remaining_seconds * 100)
            / selected_duration_seconds;

        if(percentage > 100)
        {
            percentage = 100;
        }

        lv_arc_set_value(
            ui_ArcTimer,
            static_cast<int32_t>(percentage)
        );
    }


    void update_display()
    {
        update_time_label();
        update_start_pause_label();
        update_arc();
    }


    /*
     * --------------------------------------------------
     * COUNTDOWN CALLBACK
     * --------------------------------------------------
     */

    void countdown_callback(lv_timer_t* timer)
    {
        (void)timer;

        if(timer_state != TimerController::State::RUNNING)
        {
            return;
        }

        if(remaining_seconds > 0)
        {
            remaining_seconds--;
        }

        update_display();

        /*
         * Timer completed.
         */
        if(remaining_seconds == 0)
        {
            timer_state =
                TimerController::State::STOPPED;

            if(countdown_timer != nullptr)
            {
                lv_timer_pause(countdown_timer);
            }

            update_display();

            /*
             * Later:
             * Trigger the speaker/alarm here.
             */
        }
    }
    void show_adjust_highlight()
{
    lv_obj_set_style_outline_width(
        ui_LabelTimerCountdown,
        3,
        LV_PART_MAIN
    );

    lv_obj_set_style_outline_pad(
        ui_LabelTimerCountdown,
        4,
        LV_PART_MAIN
    );

    lv_obj_set_style_outline_color(
        ui_LabelTimerCountdown,
        lv_color_hex(0xF4A7A7),
        LV_PART_MAIN
    );

    lv_obj_set_style_outline_opa(
        ui_LabelTimerCountdown,
        LV_OPA_COVER,
        LV_PART_MAIN
    );
}


void hide_adjust_highlight()
{
    lv_obj_set_style_outline_width(
        ui_LabelTimerCountdown,
        0,
        LV_PART_MAIN
    );
}
}


/*
 * ==================================================
 * PUBLIC TIMER CONTROLLER
 * ==================================================
 */

namespace TimerController
{
    void init()
    {
        duration_index = 0;

        selected_duration_seconds =
            duration_options[duration_index];

        remaining_seconds =
            selected_duration_seconds;

        timer_state =
            State::STOPPED;

        /*
         * Create the LVGL timer once.
         */
        if(countdown_timer == nullptr)
        {
            countdown_timer =
                lv_timer_create(
                    countdown_callback,
                    1000,
                    nullptr
                );
        }

        /*
         * Do not start counting immediately.
         */
        lv_timer_pause(countdown_timer);

        update_display();
    }


    void toggle()
    {
        /*
         * Do not allow Start/Pause while adjusting.
         */
        if(timer_state == State::ADJUSTING)
        {
            return;
        }

        /*
         * START
         */
        if(timer_state == State::STOPPED)
        {
            /*
             * If the previous timer finished at zero,
             * restore the selected duration before
             * starting again.
             */
            if(remaining_seconds == 0)
            {
                remaining_seconds =
                    selected_duration_seconds;
            }

            timer_state =
                State::RUNNING;

            lv_timer_resume(countdown_timer);

            update_display();

            return;
        }

        /*
         * PAUSE
         */
        if(timer_state == State::RUNNING)
        {
            timer_state =
                State::PAUSED;

            lv_timer_pause(countdown_timer);

            update_display();

            return;
        }

        /*
         * RESUME
         */
        if(timer_state == State::PAUSED)
        {
            timer_state =
                State::RUNNING;

            lv_timer_resume(countdown_timer);

            update_display();

            return;
        }
    }


    void beginAdjust()
    {
        /*
         * Do not change the duration while the
         * countdown is actively running.
         */
        if(timer_state == State::RUNNING)
        {
            return;
        }

        if(countdown_timer != nullptr)
        {
            lv_timer_pause(countdown_timer);
        }

        timer_state =
            State::ADJUSTING;

        /*
         * Show the currently selected duration.
         */
        remaining_seconds =
            selected_duration_seconds;
        
        show_adjust_highlight();

        update_display();
    }


    void nextDuration()
    {
        if(timer_state != State::ADJUSTING)
        {
            return;
        }

        duration_index++;

        /*
         * Wrap 30:00 back around to 00:30.
         */
        if(duration_index >= duration_count)
        {
            duration_index = 0;
        }

        selected_duration_seconds =
            duration_options[duration_index];

        remaining_seconds =
            selected_duration_seconds;

        update_display();
    }


    void confirmDuration()
    {
        if(timer_state != State::ADJUSTING)
        {
            return;
        }

        remaining_seconds =
            selected_duration_seconds;

        timer_state =
            State::STOPPED;
        hide_adjust_highlight();

        update_display();
    }


    void reset()
{
    if(countdown_timer != nullptr)
    {
        lv_timer_pause(countdown_timer);
    }

    remaining_seconds =
        selected_duration_seconds;

    timer_state =
        State::STOPPED;

    hide_adjust_highlight();

    update_display();
}

    State getState()
    {
        return timer_state;
    }


    bool isRunning()
    {
        return timer_state ==
               State::RUNNING;
    }


    bool isPaused()
    {
        return timer_state ==
               State::PAUSED;
    }


    bool isAdjusting()
    {
        return timer_state ==
               State::ADJUSTING;
    }


    uint32_t getRemainingSeconds()
    {
        return remaining_seconds;
    }


    uint32_t getSelectedDurationSeconds()
    {
        return selected_duration_seconds;
    }
}