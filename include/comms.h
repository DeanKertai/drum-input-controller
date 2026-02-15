#pragma once
#include <Arduino.h>

namespace Comms {

enum MessageId {
    Hit,
    Waveform,
};

void init();
void sendHit(uint8_t drumId, int velocity);
void sendWaveform(int count, byte *waveform, unsigned long *timestamps);

}; // namespace Comms
