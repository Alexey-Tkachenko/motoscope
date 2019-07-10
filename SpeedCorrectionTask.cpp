#include "SpeedCorrectionTask.h"
#include "StaticAllocActivator.h"
#include "ExpressTask.h"
#include "Pins.h"
#include "Parameters.h"
#include "Globals.h"

TASK_BEGIN(SpeedCorrectionTask, { int current; int saved; })

saved = ReadValue();
TASK_SLEEP(1000);

Globals::CorrectionValue.Set(saved);

for (;;)
{
    current = ReadValue();

    if (current >= saved + Parameters::SpeedControlSensitivity
        ||
        current <= saved - Parameters::SpeedControlSensitivity
        )
    {
        Globals::CorrectionValue.Set(saved = current);
    }

    TASK_SLEEP(5);
}

TASK_BODY_END
int ReadValue() const
{
    constexpr int bound = (1023 - 999) / 2;

    int value = analogRead((byte)Pins::Control::VelocityCorrection) - bound;
    if (value < 0) value = 0;
    if (value > 999) value = 999;
    
    return value;
}
TASK_CLASS_END

void RegisterSpeedCorrectionTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<SpeedCorrectionTask>(), TaskPriority::SensorPoll);
}