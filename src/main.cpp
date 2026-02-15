#include "comms.h"
#include "trigger_input.h"
#include "waveform.h"
#include <Arduino.h>

WaveformAnalyzer waveformAnalyzer = WaveformAnalyzer();
TriggerInput inputs[] = {
    TriggerInput(A0, &waveformAnalyzer),
};

void setup() { Comms::init(); }

void loop() {
    for (TriggerInput &input : inputs) {
        input.runChecks();
    }
}
