#include "comms.h"
#include "trigger_input.h"
#include "waveform.h"
#include <Arduino.h>

TriggerInput inputs[] = {
    TriggerInput(0, A0),
};

void setup() {
    // Set prescaler to 32 (faster analogRead()).
    // Should result in 500 kHz ADC clock (vs 125 kHz default)
    ADCSRA = (ADCSRA & 0b11111000) | 0b101;

    Comms::init();
}

void loop() {
    for (TriggerInput &input : inputs) {
        input.runChecks();
    }
}
