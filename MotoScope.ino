#include <Arduino.h>
#include "Pins.h"
#include "Scheduler.h"
#include "StaticAllocActivator.h"
#include "Globals.h"

#include "SoundTask.h"
#include "LedTask.h"
#include "SpeedCorrectionTask.h"
#include "SwitchesService.h"
#include "ButtonTask.h"
#include "PowerMonitorTask.h"

#include "WorkerTask.h"

void setup() __ATTR_NORETURN__;

static Scheduler scheduler;

void setup()
{
    Serial.begin(115200);

    RegisterSoundTask(scheduler);
    RegisterLedTask(scheduler);
    RegisterSpeedCorrectionTask(scheduler);
    RegisterSwitchesService();
    RegisterButtonTask(scheduler);
    //RegisterPowerMonitorTask(scheduler);

    RegisterWorkerTask(scheduler);

    //LedSetValue(42);
    //PlaySound(SoundType::VelocityStep);

    for (;;)
    {
        scheduler.Invoke();
    }
}

void loop()
{


}
