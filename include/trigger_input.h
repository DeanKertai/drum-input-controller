#ifndef TRIGGER_INPUT_H
#define TRIGGER_INPUT_H

#include "trigger_mode.h"

class TriggerInput {
  private:
    int inputPin;
    TriggerMode mode;

    int threshold;
    unsigned long scanModeDuration; ///< Microsends
    unsigned long rampModeDuration; ///< Microsends

    int maxScanModeValue;

    void setIdle();
    void startScanMode();
    void startRampMode();
    void handleIdleMode(int rawInputValue);
    void handleScanMode(int rawInputValue);
    void handleRampMode(int rawInputValue);

  public:
    /// @brief Constructor
    /// @param inputPin Arduino pin ID (ie: A0)
    TriggerInput(int inputPin);

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

#endif
