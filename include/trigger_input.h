#pragma once

#include "trigger_mode.h"
#include "waveform.h"

class TriggerInput {
  protected:
    /// Internal ID for this drum. Used when sending hits to user software
    uint8_t id;

  private:
    int inputPin;
    TriggerMode mode;

    int threshold;
    unsigned long scanModeDuration; ///< Microsends
    unsigned long rampModeDuration; ///< Microsends

    unsigned long modeStartTime; ///< Timestamp from micros()
    int maxScanModeValue;

    void setIdle();
    void startScanMode();
    void startRampMode();
    void handleIdleMode(int rawInputValue);
    void handleScanMode(int rawInputValue);
    void handleRampMode(int rawInputValue);

  public:
    /// @brief Constructor
    /// @param id Trigger ID (used when sending hits to user software)
    /// @param inputPin Arduino pin ID (ie: A0)
    TriggerInput(uint8_t id, int inputPin);

    /// @brief Reads the value from the input pin and performs actions
    /// according to the input's current mode
    void runChecks();

    /// @brief Updates the threshold value used for this trigger
    /// @param threshold value between 0-1023
    /// @return true if setting the threshold was successful
    bool setThreshold(int threshold);

    /// @brief Sets the scan mode duration for this pin
    /// @param duration Microsends
    /// @return true if setting the duration was successful
    bool setScanModeDuration(unsigned long duration);

    /// @brief Sets the ramp mode duration for this pin
    /// @param duration Microsends
    /// @return true if setting the duration was successful
    bool setRampModeDuration(unsigned long duration);
};
