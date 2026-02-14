#include "trigger_input.h"
#include "config.h"

#include <Arduino.h>

TriggerInput::TriggerInput(int inputPin) {
    this->inputPin = inputPin;
    this->mode = TriggerMode::Idle;
    this->threshold = 10;
    this->scanModeDuration = 50000;
    this->rampModeDuration = 200000;
    this->maxScanModeValue = 0;
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
}

void TriggerInput::setIdle() { this->mode = TriggerMode::Idle; }

void TriggerInput::startScanMode() {
    this->mode = TriggerMode::Scan;
    this->maxScanModeValue = 0;
}

void TriggerInput::handleIdleMode(int rawInputValue) {
    if (rawInputValue > this->threshold) {
        startScanMode();
    }
}

void TriggerInput::handleScanMode(int rawInputValue) {}

void TriggerInput::handleRampMode(int rawInputValue) {}

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