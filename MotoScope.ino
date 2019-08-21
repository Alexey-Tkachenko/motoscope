#include <Arduino.h>
#include "Pins.h"
#include "Scheduler.h"
#include "StaticAllocActivator.h"
#include "Globals.h"

#include "SoundTask.h"
#include "LedTask.h"
#include "SpeedCorrectionTask.h"
#include "ButtonTask.h"
#include "PowerMonitorTask.h"
#include "SwitchesTask.h"
#include "StepperTask.h"

#include "WorkerTask.h"

void setup() __ATTR_NORETURN__;

static Scheduler scheduler;

void setup()
{
    Serial.begin(115200);
    pinMode(1, OUTPUT);
    digitalWrite(1, 0);
    pinMode(0, OUTPUT);
    digitalWrite(0, 0);
    delay(1000);
    digitalWrite(0, 1);
    digitalWrite(1, 1);

    RegisterSoundTask(scheduler);
    RegisterLedTask(scheduler);
    RegisterSpeedCorrectionTask(scheduler);
    RegisterSwitchesTask(scheduler);
    RegisterButtonTask(scheduler);
    RegisterStepperTask(scheduler);
    
    RegisterPowerMonitorTask(scheduler);

    RegisterWorkerTask(scheduler);

    delay(1000);
    PlaySound(SoundType::VelocityStep);

    for (;;)
    {
        scheduler.Invoke();
    }
}

void loop()
{
}
