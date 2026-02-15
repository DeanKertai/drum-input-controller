#include "comms.h"
#include "trigger_input.h"
#include "waveform.h"
#include <Arduino.h>

TriggerInput inputs[] = {
    TriggerInput(0, A0),
};

void setup() { Comms::init(); }

void loop() {
    for (TriggerInput &input : inputs) {
        input.runChecks();
    }
}
