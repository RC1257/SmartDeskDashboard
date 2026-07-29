#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

//--------------------------------------------------
// Dashboard Screens
//--------------------------------------------------

enum class Screen
{
    Splash,
    Home,
    Timer,
    Tasks,
    Calendar,
    Quote,
    Settings
};

//--------------------------------------------------
// Functions
//--------------------------------------------------

void setCurrentScreen(Screen screen);

Screen getCurrentScreen();

#endif
