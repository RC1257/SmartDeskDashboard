#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

enum class Screen
{
    Splash,
    Home,
    Timer,
    Tasks,
    Quote,
    Settings,
    Display,
    VolumeNetwork,
    About
};

void setCurrentScreen(Screen screen);
Screen getCurrentScreen();

#endif