#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>

#include "display.h"

//--------------------------------------------------
// Display pins
//--------------------------------------------------

static constexpr int DISPLAY_MOSI = 11;
static constexpr int DISPLAY_SCLK = 12;
static constexpr int DISPLAY_CS   = 10;
static constexpr int DISPLAY_DC   = 9;
static constexpr int DISPLAY_RST  = 14;

// Landscape resolution
static constexpr uint16_t SCREEN_WIDTH  = 480;
static constexpr uint16_t SCREEN_HEIGHT = 320;

// Number of screen rows LVGL renders at one time.
// 20 rows at 32-bit color uses about 38 KB.
static constexpr uint16_t DRAW_BUFFER_ROWS = 20;

//--------------------------------------------------
// Arduino_GFX display objects
//--------------------------------------------------

static Arduino_DataBus *displayBus = new Arduino_ESP32SPI(
    DISPLAY_DC,
    DISPLAY_CS,
    DISPLAY_SCLK,
    DISPLAY_MOSI,
    GFX_NOT_DEFINED // MISO is not connected
);

static Arduino_GFX *physicalDisplay = new Arduino_ST7796(
    displayBus,
    DISPLAY_RST,
    1,      // Rotation 1 = 480 x 320 landscape
    false   // TN display, not IPS
);

//--------------------------------------------------
// LVGL objects and buffers
//--------------------------------------------------

static lv_display_t *lvglDisplay = nullptr;

// SquareLine requires LV_COLOR_DEPTH 32, so each pixel is 4 bytes.
static lv_color_t drawBuffer[SCREEN_WIDTH * DRAW_BUFFER_ROWS];

// One converted RGB565 row for the physical LCD.
static uint16_t rgb565LineBuffer[SCREEN_WIDTH];

//--------------------------------------------------
// Convert LVGL 32-bit color to RGB565
//--------------------------------------------------

static uint16_t color32ToRgb565(const lv_color32_t &color)
{
    return static_cast<uint16_t>(
        ((color.red   & 0xF8U) << 8U) |
        ((color.green & 0xFCU) << 3U) |
        ((color.blue  & 0xF8U) >> 3U)
    );
}

//--------------------------------------------------
// LVGL flush callback
//--------------------------------------------------

static void displayFlushCallback(
    lv_display_t *display,
    const lv_area_t *area,
    uint8_t *pixelMap
)
{
    const int32_t width =
        area->x2 - area->x1 + 1;

    const int32_t height =
        area->y2 - area->y1 + 1;

    const lv_color32_t *source =
        reinterpret_cast<const lv_color32_t *>(pixelMap);

    for (int32_t row = 0; row < height; ++row)
    {
        for (int32_t column = 0; column < width; ++column)
        {
            rgb565LineBuffer[column] =
                color32ToRgb565(source[row * width + column]);
        }

        physicalDisplay->draw16bitRGBBitmap(
            area->x1,
            area->y1 + row,
            rgb565LineBuffer,
            width,
            1
        );
    }

    lv_display_flush_ready(display);
}

//--------------------------------------------------
// Display initialization
//--------------------------------------------------

void initDisplay()
{
    Serial.println("Initializing LVGL...");
    lv_init();

    Serial.println("Initializing ST7796S...");

    const bool displayStarted =
        physicalDisplay->begin(10000000);

    if (!displayStarted)
    {
        Serial.println("ERROR: ST7796S initialization failed.");

        while (true)
        {
            delay(1000);
        }
    }

    physicalDisplay->fillScreen(BLACK);

    Serial.print("Physical display dimensions: ");
    Serial.print(physicalDisplay->width());
    Serial.print(" x ");
    Serial.println(physicalDisplay->height());

    if (
        physicalDisplay->width() != SCREEN_WIDTH ||
        physicalDisplay->height() != SCREEN_HEIGHT
    )
    {
        Serial.println(
            "WARNING: Display rotation did not produce 480 x 320."
        );
    }

    Serial.println("Registering display with LVGL...");

    lvglDisplay =
        lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);

    if (lvglDisplay == nullptr)
    {
        Serial.println("ERROR: LVGL display creation failed.");

        while (true)
        {
            delay(1000);
        }
    }

    lv_display_set_flush_cb(
        lvglDisplay,
        displayFlushCallback
    );

    lv_display_set_buffers(
        lvglDisplay,
        drawBuffer,
        nullptr,
        sizeof(drawBuffer),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );

    Serial.println("Display initialization complete.");
}