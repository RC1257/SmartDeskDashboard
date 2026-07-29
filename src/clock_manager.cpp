#include "../include/clock_manager.h"

#include "lvgl.h"
#include "ui.h"

#include <ctime>
#include <cstdio>

namespace
{
    lv_timer_t* clock_timer = nullptr;

    /*
     * Replace or extend these names so they exactly match ui.h.
     */
 

    void update_labels()
    {
        lv_obj_t* time_labels[] =
        {
        ui_labelClock,
        ui_LabelTimeBar1,
        ui_LabelTimeBar2,
        ui_LabelTimeBar3,
        ui_LabelTimeBar4,
        ui_LabelTimeBar5,
        ui_LabelTimeBar6
        };

        lv_obj_t* date_label =
            ui_labelDate;



        std::time_t now = std::time(nullptr);

        std::tm local_time{};

#ifdef _WIN32
        localtime_s(
            &local_time,
            &now
        );
#else
        localtime_r(
            &now,
            &local_time
        );
#endif

        char time_buffer[16];
        char date_buffer[32];

        /*
         * 12-hour time, such as 8:42 PM.
         */
        std::strftime(
            time_buffer,
            sizeof(time_buffer),
            "%I:%M %p",
            &local_time
        );

        /*
         * Date, such as Monday, July 28.
         */
        std::strftime(
            date_buffer,
            sizeof(date_buffer),
            "%A, %B %d",
            &local_time
        );

        /*
         * Remove the leading zero from times like 08:42 PM.
         */
        const char* formatted_time = time_buffer;

        if(time_buffer[0] == '0')
        {
            formatted_time =
                &time_buffer[1];
        }

        for(lv_obj_t* label : time_labels)
        {
            if(label != nullptr)
            {
                lv_label_set_text(
                    label,
                    formatted_time
                );
            }
        }

        if(date_label != nullptr)
        {
            lv_label_set_text(
                date_label,
                date_buffer
            );
        }
    }


    void clock_timer_callback(
        lv_timer_t* timer
    )
    {
        (void)timer;

        update_labels();
    }
}


namespace ClockManager
{
    void init()
    {
        /*
         * Show the correct time immediately instead of waiting
         * for the first timer callback.
         */
        update_labels();

        /*
         * Refresh every second.
         */
        clock_timer = lv_timer_create(
            clock_timer_callback,
            1000,
            nullptr
        );
    }
}