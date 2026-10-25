#include <Arduino.h>
#include <WiFi.h>
#include <time.h>


#include "dashboard_wifi.h"
#include "wifi_credentials.h"
// ----------------------------------------------------
// Wi-Fi credentials
// ----------------------------------------------------

// ----------------------------------------------------
// Time configuration
// ----------------------------------------------------

// Eastern Time with automatic daylight-saving handling.
const char* TIME_ZONE = "EST5EDT,M3.2.0,M11.1.0";

// NTP servers
const char* NTP_SERVER_1 = "pool.ntp.org";
const char* NTP_SERVER_2 = "time.nist.gov";


void initWiFi()
{
    Serial.println("Initializing WiFi...");

    WiFi.mode(WIFI_STA);

// Scan for nearby Wi-Fi networks
Serial.println("Scanning for WiFi networks...");

int networkCount = WiFi.scanNetworks();

Serial.print("Networks found: ");
Serial.println(networkCount);

for(int i = 0; i < networkCount; i++)
{
    Serial.print("Network ");
    Serial.print(i + 1);
    Serial.print(": ");

    Serial.println(WiFi.SSID(i));
}

WiFi.scanDelete();

// Attempt connection
Serial.println("Attempting WiFi connection...");

WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
);

    Serial.print("Connecting to WiFi");

    unsigned long startTime = millis();

    /*
     * Give Wi-Fi up to 15 seconds to connect.
     * We don't want the dashboard stuck forever
     * if Wi-Fi isn't available.
     */
    while(
        WiFi.status() != WL_CONNECTED &&
        millis() - startTime < 15000
    )
    {
        delay(500);
        Serial.print(".");
    }

    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println();
        Serial.println("WiFi connected.");

        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

        /*
         * Configure the ESP32 system clock.
         *
         * configTzTime automatically handles:
         * - NTP synchronization
         * - Eastern Standard Time
         * - Eastern Daylight Time
         */
        configTzTime(
    TIME_ZONE,
    NTP_SERVER_1,
    NTP_SERVER_2
);

// Explicitly apply Eastern Time to the system clock
setenv("TZ", TIME_ZONE, 1);
tzset();

Serial.println("Waiting for NTP time...");

        struct tm timeInfo;

        if(getLocalTime(&timeInfo, 20000))
        {
            Serial.println("Time synchronized.");

            Serial.print("Current time: ");
            Serial.println(
                &timeInfo,
                "%A, %B %d %Y %I:%M:%S %p"
            );
        }
        else
        {
            Serial.println(
                "WARNING: Could not synchronize time."
            );
        }
    }
    else
{
    Serial.println();
    Serial.println("WARNING: WiFi connection failed.");

    Serial.print("WiFi status code: ");
    Serial.println(WiFi.status());

    Serial.print("ESP32 MAC address: ");
    Serial.println(WiFi.macAddress());

    Serial.print("Network SSID: ");
    Serial.println(WIFI_SSID);
}
}


void updateWiFi()
{
    /*
     * Nothing needed here yet.
     *
     * The ESP32 maintains its system clock after
     * NTP synchronization.
     */
}