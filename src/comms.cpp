#include "comms.h"
#include <Arduino.h>

void Comms::init() { Serial.begin(9600); }

void Comms::sendHit(int drum, int velocity) {
    Serial.print("hit ");
    Serial.print(drum);
    Serial.print(" ");
    Serial.println(velocity);
}