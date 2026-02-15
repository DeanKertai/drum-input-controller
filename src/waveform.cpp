#include "waveform.h"
#include "comms.h"
#include <Arduino.h>

WaveformAnalyzer::WaveformAnalyzer() { this->analysisRunning = false; }

void WaveformAnalyzer::start(int inputPin) {
    this->analysisRunning = true;
    this->currentPin = inputPin;
    this->currentIndex = 0;
    this->startTime = micros();
    this->isRising = false;
    this->isFalling = false;
    this->lastValue = -1;
}

void WaveformAnalyzer::stop() { this->analysisRunning = false; }

void WaveformAnalyzer::update(int inputPin, int value) {
    if (!this->analysisRunning || inputPin != this->currentPin) {
        return;
    }

    unsigned long runningDuration = micros() - this->startTime;

    if (this->currentIndex >= Config::MAX_WAVEFORM_POINTS ||
        runningDuration > Config::MAX_WAVEFORM_DURATION) {
        this->sendWaveform();
        this->stop();
        return;
    }

    // If this is the first point, record it (don't know if it's rising or falling at this point)
    if (currentIndex == 0) {
        this->waveform[this->currentIndex] = (byte)(value >> 2);
        this->timestamps[this->currentIndex] = micros() - this->startTime;
        this->currentIndex++;
    }

    // If we were rising, and the new value is lower than the previous value, record a peak
    if (this->isRising && value < this->lastValue) {
        this->waveform[this->currentIndex] = (byte)(value >> 2);
        this->timestamps[this->currentIndex] = micros() - this->startTime;
        this->currentIndex++;
    }

    // If we were falling, and the new value is higher than the previous value, record a valley
    if (this->isFalling && value > this->lastValue) {
        this->waveform[this->currentIndex] = (byte)(value >> 2);
        this->timestamps[this->currentIndex] = micros() - this->startTime;
        this->currentIndex++;
    }

    this->isFalling = value < this->lastValue;
    this->isRising = value > this->lastValue;
    this->lastValue = value;
}

void WaveformAnalyzer::sendWaveform() {
    Comms::sendWaveform(this->currentIndex, this->waveform, this->timestamps);
}