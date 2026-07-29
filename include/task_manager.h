#pragma once

#include <stdint.h>

namespace TaskManager
{
    constexpr uint8_t TASK_COUNT = 4;

    void init();

    void toggleTask(uint8_t index);
    void setTaskComplete(uint8_t index, bool complete);
    bool isTaskComplete(uint8_t index);

    void resetAll();

    // Task text
    void setTaskText(uint8_t index, const char* text);
    const char* getTaskText(uint8_t index);
}