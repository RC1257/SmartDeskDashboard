#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>

// Initialize the MAX98357A amplifier
void initAudio();

// Play a short notification tone
void playNotificationSound();

// Set speaker volume (0–100%)
void setAudioVolume(uint8_t volume);

// Stop any currently playing sound
void stopAudio();

#endif