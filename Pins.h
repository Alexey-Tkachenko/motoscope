#pragma once
#include <Arduino.h>
#include <avr/pgmspace.h>

namespace Pins
{
    namespace Diods
    {
        const byte Anodes[] = { 6, 7, 8, 9, 10, 11, 12 };
        const byte Cathods[] = { A0, A1, A2 };
    };

    enum class Stepper : uint8_t
    {
        Velocity = A4,
        Direction = A3,
        Pulse = 13,
        Enable = 1,
    };

    enum class Control : uint8_t
    {
        VelocityCorrection = A4,
        Button = 4,
    };

    enum class Indication : uint8_t
    {
        Buzzer = 5,
    };

    enum class SwitchLimits : uint8_t
    {
        Start = 2,
        Finish = 3,
    };

    enum class Telemetry : uint8_t
    {
        Voltage = A5,
    };

}
