#pragma once

#include <stdint.h>

namespace TimerController
{
    enum class State
    {
        STOPPED,
        RUNNING,
        PAUSED,
        ADJUSTING
    };

    // Call once after ui_init().
    void init();

    // Start / Pause / Resume.
    void toggle();

    // Enter timer-adjustment mode.
    void beginAdjust();

    // Move to the next allowed duration.
    void nextDuration();

    // Accept the currently displayed duration.
    void confirmDuration();

    // Reset back to the selected starting duration.
    void reset();

    // State information.
    State getState();

    bool isRunning();
    bool isPaused();
    bool isAdjusting();

    // Time information.
    uint32_t getRemainingSeconds();
    uint32_t getSelectedDurationSeconds();
}