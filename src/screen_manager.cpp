#include <Arduino.h>

#include "screen_manager.h"
#include "state_manager.h"

//--------------------------------------------------
// Splash Screen
//--------------------------------------------------

void goToSplashScreen()
{
    setCurrentScreen(Screen::Splash);

    Serial.println("Switched to Splash Screen");
}

//--------------------------------------------------
// Home Screen
//--------------------------------------------------

void goToHomeScreen()
{
    setCurrentScreen(Screen::Home);

    Serial.println("Switched to Home Screen");
}

//--------------------------------------------------
// Timer Screen
//--------------------------------------------------

void goToTimerScreen()
{
    setCurrentScreen(Screen::Timer);

    Serial.println("Switched to Timer Screen");
}

//--------------------------------------------------
// Tasks Screen
//--------------------------------------------------

void goToTasksScreen()
{
    setCurrentScreen(Screen::Tasks);

    Serial.println("Switched to Tasks Screen");
}

//--------------------------------------------------
// Calendar Screen
//--------------------------------------------------

void goToCalendarScreen()
{
    setCurrentScreen(Screen::Calendar);

    Serial.println("Switched to Calendar Screen");
}

//--------------------------------------------------
// Quote Screen
//--------------------------------------------------

void goToQuoteScreen()
{
    setCurrentScreen(Screen::Quote);

    Serial.println("Switched to Quote Screen");
}

//--------------------------------------------------
// Settings Screen
//--------------------------------------------------

void goToSettingsScreen()
{
    setCurrentScreen(Screen::Settings);

    Serial.println("Switched to Settings Screen");
}