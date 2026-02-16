#pragma once
#include "config.h"
#include <Arduino.h>

namespace WaveformAnalyzer {

extern bool enabled;

extern byte waveform[Config::MAX_WAVEFORM_POINTS];
extern unsigned long timestamps[Config::MAX_WAVEFORM_POINTS]; ///< timestamp for each waveform point

extern int currentIndex;
extern bool analysisRunning;
extern int currentPin;          ///< Input pin that we're currently running waveform analysis on
extern unsigned long startTime; ///< When waveform analysis started (micros() timestamp)

extern int lastValue;

extern bool isFlat;
extern bool isRising;
extern bool isFalling;

void setEnabled(bool enabled);
void start(int inputPin);
void stop();
void update(int inputPin, int value);
void sendWaveform();
}; // namespace WaveformAnalyzer
