#pragma once

namespace Parameters
{
    constexpr float    AverageSpeedDelay = 0.130; /* seconds per step */
    constexpr float    ControlSpeed = 0.035 /*range*/ * 2 / 500 /*steps*/;
    constexpr unsigned SpeedIndicationTimeout = 5;

    constexpr int      LedBrightness = 10;
    constexpr bool     LedBrightnessDebug = true;

    constexpr bool     UseSound = true;

    constexpr unsigned ClickGap = 500;
    constexpr unsigned ClickShort = 1500;
    constexpr unsigned ClickLong = 3000;

    constexpr int      PowerSilentLimit = 500;
    constexpr int      PowerLowLimit = 850;
    constexpr float    PowerScale = 3.77 / (627 - 485);
    constexpr float    PowerOffset = 10 - PowerScale * 485;

}
