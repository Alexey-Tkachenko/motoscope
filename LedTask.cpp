#include "LedTask.h"

#include <avr/pgmspace.h>

#include "StaticAllocActivator.h"
#include "Parameters.h"
#include "Pins.h"
#include "ExpressTask.h"
#include "Trace.h"

static unsigned long ledOffTimeout;
static bool ledDisabled;

byte ledValue[3];

static bool lowPower;

TASK_BEGIN(LedTask, {int a; int c;})

this->Initialize();

DigitOff(0);
DigitOff(1);
DigitOff(2);

for (;;)
{
    DrawDigit(ledValue[0], 0);
    if (Parameters::LedBrightnessDebug)
    {
        TASK_SLEEP(3);
    }
    else
    {
        delayMicroseconds(Parameters::LedBrightness);
    }
    DigitOff(0);

    DrawDigit(ledValue[1], 1);
    if (Parameters::LedBrightnessDebug)
    {
        TASK_SLEEP(3);
    }
    else
    {
        delayMicroseconds(Parameters::LedBrightness);
    }
    DigitOff(1);

    DrawDigit(ledValue[2], 2);
    if (Parameters::LedBrightnessDebug)
    {
        TASK_SLEEP(3);
    }
    else
    {
        delayMicroseconds(Parameters::LedBrightness);
    }
    DigitOff(2);
    
    if (!Parameters::LedBrightnessDebug)
    {
        TASK_SLEEP(15);
    }
}

TASK_BODY_END

void Initialize() const
{
    for (byte a : Pins::Diods::Anodes)
    {
        pinMode(a, INPUT);
        digitalWrite(a, 1);
        pinMode(a, OUTPUT);
        digitalWrite(a, 0);
    }

    for (byte c : Pins::Diods::Cathods)
    {
        pinMode(c, OUTPUT);
        digitalWrite(c, 1);
    }
}

void DrawDigit(byte digit, byte index)
{
    if (digit > 9) return;
    if (index > 2) return;

    static const byte digitMask[10 + 3] PROGMEM = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x73, 0x38, 0x5C };
    byte mask;

    if (lowPower)
    {
        mask = pgm_read_byte(digitMask + 10 + index);
    }
    else
    {
        mask = ledDisabled ? 0 : pgm_read_byte(digitMask + digit);
    }

    for (byte i = 0; i < 7; ++i)
    {
        digitalWrite(Pins::Diods::Anodes[i], (mask >> i) & 1);
    }
    
    digitalWrite(Pins::Diods::Cathods[index], 0);
}

void DigitOff(byte index)
{
    if (index > 2) return;
    digitalWrite(Pins::Diods::Cathods[index], 1);
}

TASK_CLASS_END

TASK_BEGIN(LedTimeoutTask, {})

for (;;)
{
    if (millis() >= ledOffTimeout)
    {
        ledDisabled = true;
    }
    TASK_SLEEP(1000)
}

TASK_END

void RegisterLedTask(Scheduler & scheduler)
{
    scheduler.Register(Instance<LedTask>(), TaskPriority::SensorPoll);
    scheduler.Register(Instance<LedTimeoutTask>());
}

void LedSetValue(int value)
{
    Trace(F("Led"),F("SetValue"), value);

    ledOffTimeout = millis() + 1000 * Parameters::SpeedIndicationTimeout;
    ledDisabled = false;

    ledValue[2] = value % 10;
    value /= 10;
    ledValue[1] = value % 10;
    value /= 10;
    ledValue[0] = value % 10;
}

void LedLowPowerStart()
{
    lowPower = true;
}

void LedLowPowerEnd()
{
    lowPower = false;
}

