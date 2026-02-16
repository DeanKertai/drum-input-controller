#include "comms.h"
#include "config.h"
#include <Arduino.h>

void Comms::init() { Serial.begin(115200); }

void Comms::sendHit(uint8_t drumId, int velocity) {
    Serial.print("hit ");
    Serial.print(drumId);
    Serial.print(",");
    Serial.println(velocity);
}

void Comms::sendWaveform(int count, byte *waveform, unsigned long *timestamps) {
    Serial.println("waveform");
    for (int i = 0; i < count; i++) {
        Serial.print(timestamps[i]);
        Serial.print(",");
        Serial.println(waveform[i]);
    }
    Serial.println("end");
}
