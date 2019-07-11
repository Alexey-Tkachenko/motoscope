#pragma once

namespace Parameters
{
    constexpr unsigned MaxVelocityDelay = 10;
    constexpr unsigned LowVelocityDelay = 1000;
    constexpr unsigned ExtraSpeedOnStartSwitch = 256;
    constexpr unsigned AverageSpeedDelay = 100;
    constexpr float    ControlSpeed = 0.01;
    constexpr unsigned SpeedIndicationTimeout = 5;
    constexpr int      SpeedControlSensitivity = 3;

    constexpr int      LedBrightness = 4;
    constexpr bool     LedBrightnessDebug = true;

    constexpr unsigned ClickGap = 300;
    constexpr unsigned ClickShort = 1000;
    constexpr unsigned ClickLong = 2000;

    constexpr int      PowerSilentLimit = 200;
    constexpr int      PowerLowLimit = 510;
    constexpr float    PowerScale = 3.77 / (627 - 485);
    constexpr float    PowerOffset = 10 - PowerScale * 485;
}
