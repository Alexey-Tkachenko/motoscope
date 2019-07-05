#pragma once
#include <Arduino.h>

namespace Pins
{
    namespace Diods
    {
        enum class Anodes
        {
            A1 = 6,
            A2 = 7,
            A3 = 8,
            A4 = 9,
            A5 = 10,
            A6 = 11,
            A7 = 12
        };

        enum class Cathods
        {
            C0 = A0,
            C1 = A1,
            C2 = A2,
        };
    };

    enum class Stepper
    {
        Velocity = A4,
        Direction = A3,
        Pulse = 13,
    };

    enum class Control
    {
        VelocotyCorrection = A4,
        Button = 4,
    };

    enum class Indication
    {
        Buzzer = 5,
    };

    enum class SwitchLimits
    {
        Start = 2,
        Finish = 3,
    };

    enum class Telemetry
    {
        Voltage = A5,
    };

}
