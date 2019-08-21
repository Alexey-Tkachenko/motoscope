#include "WorkerTask.h"
#include "ExpressTask.h"
#include "SoundTask.h"
#include "LedTask.h"
#include "PeriodicTimer.h"
#include "StaticAllocActivator.h"
#include "Globals.h"
#include "ButtonTask.h"
#include "StepperTask.h"
#include "Trace.h"

enum class Substate
{
    Idle,
    Tracking,
    Complete,
    Suspended,
    Parking,
    Parked,
};

static const __FlashStringHelper* cat()
{
    return F("Worker");
}

TASK_BEGIN(WorkerTask, { int value; Substate substate; })

Trace(cat(), F("Initial Idle"));

for (;;)
{
    if (substate == Substate::Idle)
    {
        Trace(cat(), F("Idle"));
        WaitClick(true, true);
        for (;;)
        {
            if (CheckReset(Globals::ClickShort))
            {
                Trace(cat(), F("Idle->Tracking"));
                StepperMoveForward();
                substate = Substate::Tracking;
                break;
            }
            if (CheckReset(Globals::ClickLong))
            {
                Trace(cat(), F("Idle->Parking"));
                StepperMoveBackward();
                substate = Substate::Parking;
                break;
            }
            TASK_YIELD();
        }
    }
    else if (substate == Substate::Tracking)
    {
        Trace(cat(), F("Tracking"));
        WaitClick(true, true);
        for (;;)
        {
            if (CheckReset(Globals::ClickShort))
            {
                Trace(cat(), F("Tracking->Suspended"));
                StepperStop();
                substate = Substate::Suspended;
                break;
            }
            if (CheckReset(Globals::ClickLong))
            {
                Trace(cat(), F("Tracking->Parking"));
                StepperMoveBackward();
                substate = Substate::Parking;
                break;
            }
            if (CheckReset(Globals::SwitchFinishClose))
            {
                Trace(cat(), F("Tracking->Complete"));
                StepperMoveBackward();
                while (CheckReset(Globals::SwitchFinishOpen) == false)
                {
                    TASK_YIELD();
                }
                StepperStop();
                break;
            }
            TASK_YIELD();
        }
    }
    else if (substate == Substate::Complete)
    {
        Trace(cat(), F("Complete"));
        WaitClick(true, false);
        for (;;)
        {
            if (CheckReset(Globals::ClickLong))
            {
                Trace(cat(), F("Complete->Parking"));
                StepperMoveBackward();
                substate = Substate::Parking;
                break;
            }
            TASK_YIELD();
        }
    }
    else if (substate == Substate::Suspended)
    {
        Trace(cat(), F("Suspended"));
        WaitClick(true, true);
        for (;;)
        {
            if (CheckReset(Globals::ClickShort))
            {
                Trace(cat(), F("Suspended->Tracking"));
                StepperMoveForward();
                substate = Substate::Tracking;
                break;
            }
            if (CheckReset(Globals::ClickLong))
            {
                Trace(cat(), F("Suspended->Parking"));
                StepperMoveBackward();
                substate = Substate::Parking;
                break;
            }
            TASK_YIELD();
        }
    }
    else if (substate == Substate::Parking)
    {
        Trace(cat(), F("Parking"));
        WaitClick(true, true);
        for (;;)
        {
            if (CheckReset(Globals::ClickShort) || CheckReset(Globals::ClickLong))
            {
                Trace(cat(), F("Parking->Idle"));
                StepperStop();
                substate = Substate::Idle;
                break;
            }
            if (CheckReset(Globals::SwitchStartClose))
            {
                Trace(cat(), F("Parking->Parked"));
                StepperFastForward();
                while (CheckReset(Globals::SwitchStartOpen) == false)
                {
                    TASK_YIELD();
                }
                StepperStop();
                substate = Substate::Parked;
                break;
            }
            TASK_YIELD();
        }
    }
    else if (substate == Substate::Parked)
    {
        Trace(cat(), F("Parked"));
        WaitClick(true, true);
        for (;;) 
        {
            if (CheckReset(Globals::ClickShort) || CheckReset(Globals::ClickLong))
            {
                StepperMoveForward();
                substate = Substate::Tracking;
                break;
            }
            TASK_YIELD();
        }
    }
    else
    {
        Trace(cat(), F("UnknownState"));
        abort();
    }
}

TASK_BODY_END


TASK_CLASS_END



void RegisterWorkerTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<WorkerTask>());
}