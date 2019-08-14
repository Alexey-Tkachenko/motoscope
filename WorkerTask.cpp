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
pinMode(13, OUTPUT);

for (;;)
{
    digitalWrite(13, 1);
    TASK_SLEEP(1);
    digitalWrite(13, 0);
    TASK_SLEEP(1);
}

TASK_END



void RegisterWorkerTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<WorkerTask>());
}