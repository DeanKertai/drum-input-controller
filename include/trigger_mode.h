#pragma once

enum class TriggerMode {
    /// Waiting for `threshold` value to be exceeded on this pin
    Idle,

    /// Threshold value has been exceeded. During scan mode, additional
    /// inputs are blocked on this pin, and we analyze the waveform to
    /// determine the velocity and frequency
    Scan,

    /// Ramp mode starts after scan mode. During ramp mode, a dynamic
    /// threshold reduces from the max input value during scan mode to
    /// the original `threshold` value. Any input values below this
    /// dynamic threshold are ignored. Any values above the dynamic
    /// threshold will trigger a new hit and restart scan mode
    Ramp,
};
