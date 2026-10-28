#include <Arduino.h>
#include "audio.h"
#include <driver/i2s.h>
#include <math.h>

// MAX98357A amplifier connections
#define I2S_DIN   38
#define I2S_BCLK  39
#define I2S_LRC   40

#define SAMPLE_RATE 16000
#define TONE_FREQ   440
#define AMPLITUDE   1500

static bool audioInitialized = false;
static uint8_t audioVolume = 100;

void initAudio()
{
    Serial.println("Starting MAX98357A audio test...");

    i2s_config_t i2s_config = {};

    i2s_config.mode =
        (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);

    i2s_config.sample_rate = SAMPLE_RATE;
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    i2s_config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    i2s_config.intr_alloc_flags = 0;
    i2s_config.dma_buf_count = 8;
    i2s_config.dma_buf_len = 64;
    i2s_config.use_apll = false;
    i2s_config.tx_desc_auto_clear = true;

    i2s_pin_config_t pin_config = {};

    pin_config.bck_io_num = I2S_BCLK;
    pin_config.ws_io_num = I2S_LRC;
    pin_config.data_out_num = I2S_DIN;
    pin_config.data_in_num = I2S_PIN_NO_CHANGE;

    esp_err_t result = i2s_driver_install(
        I2S_NUM_0,
        &i2s_config,
        0,
        NULL
    );

    if (result != ESP_OK)
    {
        Serial.printf("I2S driver error: %d\n", result);
        return;
    }

    result = i2s_set_pin(I2S_NUM_0, &pin_config);

    if (result != ESP_OK)
    {
        Serial.printf("I2S pin error: %d\n", result);
        return;
    }

    audioInitialized = true;

    Serial.println("I2S initialized successfully!");
}

void playNotificationSound()
{
    if (!audioInitialized || audioVolume == 0)
        return;

    const int samples = 256;
    int16_t audio_buffer[samples * 2];

    for (int i = 0; i < samples; i++)
    {
        float t = (float)i / SAMPLE_RATE;

        int16_t sample = (int16_t)(
            AMPLITUDE *
            (audioVolume / 100.0f) *
            sinf(2.0f * PI * TONE_FREQ * t)
        );

        audio_buffer[i * 2] = sample;
        audio_buffer[i * 2 + 1] = sample;
    }

    size_t bytes_written = 0;

    i2s_write(
        I2S_NUM_0,
        audio_buffer,
        sizeof(audio_buffer),
        &bytes_written,
        portMAX_DELAY
    );
}

void setAudioVolume(uint8_t volume)
{
    audioVolume = (volume > 100) ? 100 : volume;
}

void stopAudio()
{
    if (audioInitialized)
    {
        i2s_zero_dma_buffer(I2S_NUM_0);
    }
}