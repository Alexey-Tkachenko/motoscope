#pragma once

namespace Parameters
{
    constexpr float    AverageSpeedDelay = 0.17;
    constexpr float    ControlSpeed = 0.0001;
    constexpr unsigned SpeedIndicationTimeout = 5;

    constexpr int      LedBrightness = 100;
    constexpr bool     LedBrightnessDebug = true;

    constexpr unsigned ClickGap = 500;
    constexpr unsigned ClickShort = 1500;
    constexpr unsigned ClickLong = 3000;

    constexpr int      PowerSilentLimit = 200;
    constexpr int      PowerLowLimit = 510;
    constexpr float    PowerScale = 3.77 / (627 - 485);
    constexpr float    PowerOffset = 10 - PowerScale * 485;
}
