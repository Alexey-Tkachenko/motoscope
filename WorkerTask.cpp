#include "WorkerTask.h"
#include "ExpressTask.h"
#include "SoundTask.h"
#include "LedTask.h"
#include "PeriodicTimer.h"
#include "StaticAllocActivator.h"
#include "Globals.h"
#include "ButtonTask.h"

TASK_BEGIN(WorkerTask, { int value; })


WaitClick(true, true);

for (;;)
{
    TASK_WAIT_VALUE(&Globals::CorrectionValue, value);
    LedSetValue(value);
    PlaySound(SoundType::VelocityStep);

}

TASK_END



void RegisterWorkerTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<WorkerTask>());
}