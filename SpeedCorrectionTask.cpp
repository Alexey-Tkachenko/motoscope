#include "SpeedCorrectionTask.h"
#include "StaticAllocActivator.h"
#include "ExpressTask.h"
#include "Pins.h"
#include "Globals.h"
#include "LedTask.h"
#include "SoundTask.h"
#include "Trace.h"
#include "Kalman.h"
#include "Average.h"
#include "Median.h"
#include "StepperTask.h"
#include "Parameters.h"

static Kalman<int> filter(0.02);
static Average<int, 8, byte> filterA;
static Median<int, 7, byte> filterB;

TASK_BEGIN(SpeedCorrectionTask, { int current; int saved; unsigned long changed; unsigned long now; })

saved = ReadValue();
filterA.fill(saved);
filterB.fill(saved);
filter.fill(saved);
TASK_SLEEP(1000);
LedSetValue(saved);

StepperSetPeriod(Parameters::AverageSpeedDelay + (saved - 500) * Parameters::ControlSpeed);
changed = millis();

for (;;)
{
    filterA.write(ReadValue());
    filterB.write(filterA.get());
    current = filter(filterB.get());
    now = millis();

    if (Difference(saved, current) > 1 || now - changed > 200)
    {
        if (saved != current)
        {
            saved = current;
            StepperSetPeriod(Parameters::AverageSpeedDelay + (saved - 500) * Parameters::ControlSpeed);
            Trace(F("Speed"), F("Value"), current);
            LedSetValue(current);
            PlaySound(SoundType::VelocityStep);
        }
        changed = now;
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

template<class T>
T Difference(T a, T b)
{
    T d = a - b;
    if (d < 0) d = -d;
    return d;
}

TASK_CLASS_END

void RegisterSpeedCorrectionTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<SpeedCorrectionTask>(), TaskPriority::SensorPoll);
}