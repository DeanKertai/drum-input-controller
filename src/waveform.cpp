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

int WaveformAnalyzer::lastValue = 0;
bool WaveformAnalyzer::isFlat = false;
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
    isFlat = false;
    isRising = false;
    isFalling = false;
    lastValue = -1;
}

void WaveformAnalyzer::stop() { analysisRunning = false; }

void WaveformAnalyzer::update(int inputPin, int value) {
    if (!enabled || !analysisRunning || inputPin != currentPin) {
        return;
    }

    unsigned long now = micros();
    unsigned long runningDuration = now - startTime;

    if (currentIndex >= Config::MAX_WAVEFORM_POINTS ||
        runningDuration > Config::MAX_WAVEFORM_DURATION) {
        sendWaveform();
        stop();
        return;
    }

    bool isFirstPoint = currentIndex == 0;
    bool wasFlat = isFlat && value != lastValue;
    bool wasRising = isRising && value <= lastValue;
    bool wasFalling = isFalling && value >= lastValue;

    if (isFirstPoint || wasFlat || wasRising || wasFalling) {
        waveform[currentIndex] = (byte)(value >> 2);
        timestamps[currentIndex] = now - startTime;
        currentIndex++;
    }

    isFalling = value < lastValue;
    isRising = value > lastValue;
    lastValue = value;
}

void WaveformAnalyzer::sendWaveform() { Comms::sendWaveform(currentIndex, waveform, timestamps); }