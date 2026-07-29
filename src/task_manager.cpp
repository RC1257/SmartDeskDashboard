#include "../include/task_manager.h"

#include "lvgl.h"
#include "ui.h"

#include <string>

namespace
{
    bool task_complete[TaskManager::TASK_COUNT] =
    {
        false,
        false,
        false,
        false
    };

    /*
     * Temporary task data for the PC simulator.
     *
     * Later, these can be replaced with tasks received
     * by the ESP32 or loaded from saved storage.
     */
    std::string task_text[TaskManager::TASK_COUNT] =
    {
        "Finish circuits homework",
        "Email professor",
        "Review research notes",
        "Workout"
    };


    lv_obj_t* get_checkbox(uint8_t index)
    {
        switch(index)
        {
            case 0:
                return ui_Checkbox1;

            case 1:
                return ui_Checkbox2;

            case 2:
                return ui_Checkbox3;

            case 3:
                return ui_Checkbox4;

            default:
                return nullptr;
        }
    }


    lv_obj_t* get_task_label(uint8_t index)
    {
        switch(index)
        {
            case 0:
                return ui_LabelToDo1;

            case 1:
                return ui_LabelToDo2;

            case 2:
                return ui_LabelToDo3;

            case 3:
                return ui_LabelToDo4;

            default:
                return nullptr;
        }
    }


    void update_checkbox(uint8_t index)
    {
        if(index >= TaskManager::TASK_COUNT)
        {
            return;
        }

        lv_obj_t* checkbox =
            get_checkbox(index);

        if(checkbox == nullptr)
        {
            return;
        }

        if(task_complete[index])
        {
            lv_obj_add_state(
                checkbox,
                LV_STATE_CHECKED
            );
        }
        else
        {
            lv_obj_remove_state(
                checkbox,
                LV_STATE_CHECKED
            );
        }
    }


    void update_task_label(uint8_t index)
    {
        if(index >= TaskManager::TASK_COUNT)
        {
            return;
        }

        lv_obj_t* label =
            get_task_label(index);

        if(label == nullptr)
        {
            return;
        }

        lv_label_set_text(
            label,
            task_text[index].c_str()
        );
    }


    void update_all()
    {
        for(uint8_t i = 0;
            i < TaskManager::TASK_COUNT;
            i++)
        {
            update_checkbox(i);
            update_task_label(i);
        }
    }
}


namespace TaskManager
{
    void init()
    {
        for(uint8_t i = 0;
            i < TASK_COUNT;
            i++)
        {
            task_complete[i] = false;
        }

        update_all();
    }


    void toggleTask(uint8_t index)
    {
        if(index >= TASK_COUNT)
        {
            return;
        }

        task_complete[index] =
            !task_complete[index];

        update_checkbox(index);
    }


    void setTaskComplete(
        uint8_t index,
        bool complete
    )
    {
        if(index >= TASK_COUNT)
        {
            return;
        }

        task_complete[index] =
            complete;

        update_checkbox(index);
    }


    bool isTaskComplete(uint8_t index)
    {
        if(index >= TASK_COUNT)
        {
            return false;
        }

        return task_complete[index];
    }


    void resetAll()
    {
        for(uint8_t i = 0;
            i < TASK_COUNT;
            i++)
        {
            task_complete[i] = false;
        }

        update_all();
    }


    void setTaskText(
        uint8_t index,
        const char* text
    )
    {
        if(index >= TASK_COUNT ||
           text == nullptr)
        {
            return;
        }

        task_text[index] = text;

        update_task_label(index);
    }


    const char* getTaskText(uint8_t index)
    {
        if(index >= TASK_COUNT)
        {
            return "";
        }

        return task_text[index].c_str();
    }
}