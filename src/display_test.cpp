#include <Arduino.h>
#include <Arduino_GFX_Library.h>

static constexpr int TFT_MOSI = 11;
static constexpr int TFT_SCLK = 12;
static constexpr int TFT_CS   = 10;
static constexpr int TFT_DC   = 9;
static constexpr int TFT_RST  = 14;

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    TFT_DC,
    TFT_CS,
    TFT_SCLK,
    TFT_MOSI,
    GFX_NOT_DEFINED
);

Arduino_GFX *display = new Arduino_ST7796(
    bus,
    TFT_RST,
    1,      // landscape rotation
    false   // IPS setting; your display is TN
);

void setup()
{
    Serial.begin(115200);
    delay(1500);

    Serial.println("Starting Arduino_GFX display test...");

    if (!display->begin(10000000))
    {
        Serial.println("Display initialization failed.");
        while (true)
        {
            delay(1000);
        }
    }

    Serial.printf(
        "Display dimensions: %d x %d\n",
        display->width(),
        display->height()
    );

    display->fillScreen(RGB565_RED);
    delay(1000);

    display->fillScreen(RGB565_GREEN);
    delay(1000);

    display->fillScreen(RGB565_BLUE);
    delay(1000);

    display->fillScreen(RGB565_BLACK);

    display->setTextColor(RGB565_WHITE);
    display->setTextSize(3);
    display->setCursor(55, 145);
    display->println("Smart Desk Dashboard");

    Serial.println("Display test complete.");
}

void loop()
{
    delay(1000);
}