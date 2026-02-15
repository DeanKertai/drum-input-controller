#include "comms.h"
#include "config.h"
#include <Arduino.h>

void Comms::init() { Serial.begin(9600); }

void Comms::sendHit(int drum, int velocity) {
    Serial.print("hit ");
    Serial.print(drum);
    Serial.print(" ");
    Serial.println(velocity);
}

void Comms::sendWaveform(int count, byte *waveform, unsigned long *timestamps) {
    Serial.println("waveform");
    for (int i = 0; i < count; i++) {
        Serial.print(waveform[i]);
        Serial.print(",");
        Serial.println(timestamps[i]);
    }
    Serial.println("end");
}
