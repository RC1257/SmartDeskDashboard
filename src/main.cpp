#include <Arduino.h>

/**
 * Smart Desk Dashboard
 * 
 * This program initializes the serial communication and prints boot messages.
 * It then continuously prints "System Running" every second.
 */
/**
void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" Smart Desk Dashboard Booting...");
    Serial.println("=================================");
}

void loop()
{
    Serial.println("System Running");

    delay(1000);
}
*/

#include "display.h"
#include "screen_manager.h"
#include "navigation.h"
#include "timer.h"
#include "clock.h"
#include "settings.h"
#include "tasks.h"
#include "quote.h"
#include "storage.h"
#include "wifi.h"
#include "ui_update.h"
#include "state_manager.h"


void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("===============================");
    Serial.println(" RC Smart Desk Dashboard");
    Serial.println(" Boot Sequence");
    Serial.println("===============================");

    initDisplay();
    initStorage();
    initSettings();
    initClock();
    initWiFi();
    initTimer();
    initTasks();
    // initQuotes();

    initUI();
    Navigation::init();

    goToSplashScreen();
    
}

void loop()
{
   
    updateClock();

    updateTimer();

    updateWiFi();

    updateUI();

    delay(5);
}


