#include "waveform.h"
#include "comms.h"
#include <Arduino.h>

bool WaveformAnalyzer::enabled = false;
byte WaveformAnalyzer::waveform[Config::MAX_WAVEFORM_POINTS];
unsigned long WaveformAnalyzer::timestamps[Config::MAX_WAVEFORM_POINTS];
int WaveformAnalyzer::currentIndex;
bool WaveformAnalyzer::analysisRunning = false;
int WaveformAnalyzer::currentPin;
unsigned long WaveformAnalyzer::startTime;
int WaveformAnalyzer::lastValue = -1;
bool WaveformAnalyzer::isRising = false;
bool WaveformAnalyzer::isFalling = false;

void WaveformAnalyzer::setEnabled(bool enabled) { WaveformAnalyzer::enabled = enabled; }

void WaveformAnalyzer::start(int inputPin) {
    if (!enabled) {
        return;
    }
    analysisRunning = true;
    currentPin = inputPin;
    currentIndex = 0;
    startTime = micros();
    isRising = false;
    isFalling = false;
    lastValue = -1;
}

void WaveformAnalyzer::stop() { analysisRunning = false; }

void WaveformAnalyzer::update(int inputPin, int value) {
    if (!enabled || !analysisRunning || inputPin != currentPin) {
        return;
    }

    unsigned long runningDuration = micros() - startTime;

    if (currentIndex >= Config::MAX_WAVEFORM_POINTS ||
        runningDuration > Config::MAX_WAVEFORM_DURATION) {
        sendWaveform();
        stop();
        return;
    }

    // If this is the first point, record it (don't know if it's rising or falling at this point)
    if (currentIndex == 0) {
        waveform[currentIndex] = (byte)(value >> 2);
        timestamps[currentIndex] = micros() - startTime;
        currentIndex++;
    }

    // If we were rising, and the new value is lower than the previous value, record a peak
    if (isRising && value < lastValue) {
        waveform[currentIndex] = (byte)(value >> 2);
        timestamps[currentIndex] = micros() - startTime;
        currentIndex++;
    }

    // If we were falling, and the new value is higher than the previous value, record a valley
    if (isFalling && value > lastValue) {
        waveform[currentIndex] = (byte)(value >> 2);
        timestamps[currentIndex] = micros() - startTime;
        currentIndex++;
    }

    isFalling = value < lastValue;
    isRising = value > lastValue;
    lastValue = value;
}

void WaveformAnalyzer::sendWaveform() { Comms::sendWaveform(currentIndex, waveform, timestamps); }