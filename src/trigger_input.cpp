#include "trigger_input.h"
#include "comms.h"
#include "config.h"

#include <Arduino.h>

TriggerInput::TriggerInput(int inputPin, WaveformAnalyzer *waveformAnalyzer) {
    this->inputPin = inputPin;
    this->waveformAnalyzer = waveformAnalyzer;
    this->mode = TriggerMode::Idle;
    this->threshold = 10;
    this->scanModeDuration = 50000;
    this->rampModeDuration = 200000;
    this->maxScanModeValue = 0;
    this->modeStartTime = 0;
}

void TriggerInput::runChecks() {
    int rawInputValue = analogRead(this->inputPin);

    switch (this->mode) {
    case TriggerMode::Idle:
        this->handleIdleMode(rawInputValue);
        break;

    case TriggerMode::Scan:
        this->handleScanMode(rawInputValue);
        break;

    case TriggerMode::Ramp:
        this->handleRampMode(rawInputValue);
        break;
    }

    this->waveformAnalyzer->update(this->inputPin, rawInputValue);
}

void TriggerInput::setIdle() {
    this->modeStartTime = micros();
    this->mode = TriggerMode::Idle;
}

void TriggerInput::handleIdleMode(int rawInputValue) {
    if (rawInputValue > this->threshold) {
        this->startScanMode();
    }
}

void TriggerInput::startScanMode() {
    this->mode = TriggerMode::Scan;
    this->modeStartTime = micros();
    this->maxScanModeValue = 0;
    this->waveformAnalyzer->start(this->inputPin);
}

void TriggerInput::handleScanMode(int rawInputValue) {
    if (micros() - this->modeStartTime >= this->scanModeDuration) {
        Comms::sendHit(this->inputPin, this->maxScanModeValue);
        this->startRampMode();
        return;
    }
    if (rawInputValue > this->maxScanModeValue) {
        this->maxScanModeValue = rawInputValue;
    }
}

void TriggerInput::startRampMode() {
    if (this->rampModeDuration == 0) {
        this->setIdle();
        return;
    }
    this->modeStartTime = micros();
    this->mode = TriggerMode::Ramp;
}

void TriggerInput::handleRampMode(int rawInputValue) {
    unsigned long timeSinceStart = micros() - this->modeStartTime;
    if (timeSinceStart >= this->rampModeDuration) {
        this->setIdle();
        return;
    }
    float percentComplete = (float)timeSinceStart / (float)this->rampModeDuration;
    float rampHeight = (float)(this->maxScanModeValue - this->threshold);
    int dynamicThreshold = this->maxScanModeValue - (int)(rampHeight * percentComplete);

    // Allow new hits if input value exceeds the dynamic ramp threshold
    if (rawInputValue >= dynamicThreshold) {
        this->startScanMode();
    }
}

bool TriggerInput::setThreshold(int threshold) {
    if (threshold < 0 || threshold > Config::MAX_INPUT_VALUE) {
        return false;
    }
    this->threshold = threshold;
    return true;
}

bool TriggerInput::setScanModeDuration(unsigned long duration) {
    if (duration > Config::MAX_MODE_DURATION) {
        return false;
    }
    this->scanModeDuration = duration;
    return true;
}

bool TriggerInput::setRampModeDuration(unsigned long duration) {
    if (duration > Config::MAX_MODE_DURATION) {
        return false;
    }
    this->rampModeDuration = duration;
    return true;
}