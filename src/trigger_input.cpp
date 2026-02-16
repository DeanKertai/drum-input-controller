#include "trigger_input.h"
#include "comms.h"
#include "config.h"

#include <Arduino.h>

TriggerInput::TriggerInput(uint8_t id, int inputPin) {
    this->id = id;
    this->inputPin = inputPin;
    this->mode = TriggerMode::Idle;
    this->threshold = 10;
    this->scanModeDuration = 10000;
    this->blockModeDuration = 10000;
    this->rampModeDuration = 350000;
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

    case TriggerMode::Block:
        this->handleBlockMode();
        break;

    case TriggerMode::Ramp:
        this->handleRampMode(rawInputValue);
        break;
    }

    WaveformAnalyzer::update(this->inputPin, rawInputValue);
}

void TriggerInput::setIdle() {
    this->modeStartTime = micros();
    this->mode = TriggerMode::Idle;
}

void TriggerInput::handleIdleMode(int rawInputValue) {
    if (rawInputValue > this->threshold) {
        this->startScanMode(rawInputValue);
    }
}

void TriggerInput::startScanMode(int rawInputValue) {
    this->mode = TriggerMode::Scan;
    this->modeStartTime = micros();
    this->maxScanModeValue = rawInputValue;
    WaveformAnalyzer::start(this->inputPin);
}

void TriggerInput::handleScanMode(int rawInputValue) {
    if (micros() - this->modeStartTime >= this->scanModeDuration) {
        Comms::sendHit(this->id, this->maxScanModeValue);
        this->startBlockMode();
        return;
    }
    if (rawInputValue > this->maxScanModeValue) {
        this->maxScanModeValue = rawInputValue;
    }
}

void TriggerInput::startBlockMode() {
    if (this->blockModeDuration == 0) {
        this->startRampMode();
        return;
    }
    this->modeStartTime = micros();
    this->mode = TriggerMode::Block;
}

void TriggerInput::handleBlockMode() {
    if (micros() - this->modeStartTime >= this->blockModeDuration) {
        this->startRampMode();
        return;
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
        this->startScanMode(rawInputValue);
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

bool TriggerInput::setBlockModeDuration(unsigned long duration) {
    if (duration > Config::MAX_MODE_DURATION) {
        return false;
    }
    this->blockModeDuration = duration;
    return true;
}

bool TriggerInput::setRampModeDuration(unsigned long duration) {
    if (duration > Config::MAX_MODE_DURATION) {
        return false;
    }
    this->rampModeDuration = duration;
    return true;
}