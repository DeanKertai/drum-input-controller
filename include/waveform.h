#pragma once
#include "config.h"
#include <Arduino.h>

class WaveformAnalyzer {
  private:
    byte waveform[Config::MAX_WAVEFORM_POINTS];
    unsigned long timestamps[Config::MAX_WAVEFORM_POINTS]; ///< timestamp for each waveform point

    int currentIndex;
    bool analysisRunning;
    int currentPin;          ///< Input pin that we're currently running waveform analysis on
    unsigned long startTime; ///< When waveform analysis started (micros() timestamp)

    int lastValue;
    bool isRising;
    bool isFalling;

    void sendWaveform();

  public:
    WaveformAnalyzer();
    void start(int inputPin);
    void stop();
    void update(int inputPin, int value);
};
