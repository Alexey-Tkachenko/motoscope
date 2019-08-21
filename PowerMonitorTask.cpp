#include <Arduino.h>
#include "PowerMonitorTask.h"
#include "ExpressTask.h"
#include "StaticAllocActivator.h"
#include "Parameters.h"
#include "Pins.h"
#include "SoundTask.h"
#include "LedTask.h"
#include "Trace.h"

static const __FlashStringHelper* cat()
{
    return F("Power");
}


TASK_BEGIN(PowerMonitorTask, { int value; int power; }) 

for (;;)
{
    value = analogRead((uint8_t)Pins::Telemetry::Voltage);

    Trace(cat(), F("Value"), value);

    power = (int)(Parameters::PowerScale * 10 * value + 10 * Parameters::PowerOffset);

    if (value < Parameters::PowerSilentLimit)
    {
        LedLowPowerEnd();
        TASK_SLEEP(1000);
    }
    else if (value < Parameters::PowerLowLimit)
    {
        LedLowPowerStart();
        Trace(cat(), F("Low"));

        PlaySound(SoundType::PowerAlarm1);
        TASK_SLEEP(500);
        PlaySound(SoundType::PowerAlarm2);
        TASK_SLEEP(500);
    }
    else
    {
        LedLowPowerEnd();
        TASK_SLEEP(1000);
    }
}


TASK_END

void RegisterPowerMonitorTask(Scheduler &scheduler)
{
    scheduler.Register(Instance<PowerMonitorTask>(), TaskPriority::SensorPoll);
}
