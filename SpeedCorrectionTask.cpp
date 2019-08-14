#include "SpeedCorrectionTask.h"
#include "StaticAllocActivator.h"
#include "ExpressTask.h"
#include "Pins.h"
#include "Globals.h"
#include "LedTask.h"
#include "SoundTask.h"
#include "Trace.h"
#include "Kalman.h"

static Kalman<int> filter(0.075);

TASK_BEGIN(SpeedCorrectionTask, { int current; int saved; unsigned long changed; unsigned long now; })

saved = ReadValue();
TASK_SLEEP(1000);

Globals::CorrectionValue.Set(saved);
changed = millis();

for (;;)
{
    current = filter(ReadValue());
    now = millis();

    if (Difference(saved, current) > 1 || now - changed > 200)
    {
        if (saved != current)
        {
            Globals::CorrectionValue.Set(saved = current);
            Trace(F("Speed\tValue "), current);
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