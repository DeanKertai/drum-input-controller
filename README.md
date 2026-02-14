# Drum Module Input Controller

This is the low level controller that reads raw voltages from the drum triggers and attempts to parse them in to individual "drum hits" by using thresholds, masking timeouts, and waveform analysis. The drum hits are sent to the drum module software via serial, where the hits are converted in to midi notes based on user settings.

```
┌───────┐  ┌───────┐
│ piezo │  │ piezo │
│trigger│  │trigger│ ...etc
└──┬────┘  └───┬───┘
   │           │
   │ 0-5V      │0-5V
   ▼           ▼
┌──────────────────┐
│    Drum Input    │
│    Controller    │ <-- This repo
│    (Arduino)     │
└──────────────────┘
        ▲
        │ Serial
        ▼
┌──────────────────┐
│                  │
│   Drum Module    │
│   UI Software    │
│  (Raspberry Pi)  │
│                  │
└──────────────────┘
```
