#include "trigger_input.h"
#include <Arduino.h>

TriggerInput inputs[] = {
    TriggerInput(A0),
};

void setup() {}

void loop() {
    for (TriggerInput input : inputs) {
        input.runChecks();
    }
}
