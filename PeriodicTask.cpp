#include "PeriodicTask.h"
#include "ExpressTask.h"
#include "SoundTask.h"
#include "LedTask.h"
#include "PeriodicTimer.h"
#include "StaticAllocActivator.h"
#include "Globals.h"


TASK_BEGIN(PeriodicTask, { PeriodicTimer timer = PeriodicTimer(25, false); unsigned i; int value; } loc)

loc.timer.Start();

for (loc.i = 0; ; ++loc.i)
{
    TASK_WAIT_VALUE(&Globals::CorrectionValue, loc.value);
    LedSetValue(loc.value);
    PlaySound(SoundType::VelocityStep);

    //TASK_WAIT_FOR(&loc.timer);
    //LedSetValue(loc.i);
}

TASK_END



void RegisterPeriodicTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<PeriodicTask>());
}