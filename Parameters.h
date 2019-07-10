#pragma once

namespace Parameters
{
    constexpr unsigned MaxVelocityDelay = 10;
    constexpr unsigned LowVelocityDelay = 1000;
    constexpr unsigned ExtraSpeedOnStartSwitch = 256;
    constexpr unsigned AverageSpeedDelay = 100;
    constexpr float    ControlSpeed = 0.01;
    constexpr unsigned SpeedIndicationTimeout = 5;
    constexpr int      SpeedControlSensitivity = 2;

    constexpr int      LedBrightness = 400;
    constexpr bool     LedBrightnessDebug = true;
}
