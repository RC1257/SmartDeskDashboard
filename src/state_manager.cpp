#include "state_manager.h"

//--------------------------------------------------
// Current Screen
//--------------------------------------------------

static Screen currentScreen = Screen::Splash;

//--------------------------------------------------
// Set Screen
//--------------------------------------------------

void setCurrentScreen(Screen screen)
{
    currentScreen = screen;
}

//--------------------------------------------------
// Get Screen
//--------------------------------------------------

Screen getCurrentScreen()
{
    return currentScreen;
}

