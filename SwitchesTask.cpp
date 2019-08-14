#include "SwitchesTask.h"
#include "ExpressTask.h"
#include "Pins.h"
#include "Globals.h"
#include "SoundTask.h"
#include "StaticAllocActivator.h"
#include "Trace.h"

#include <Arduino.h>

TASK_BEGIN(SwitchTask, {bool cur; bool prev;})

pinMode(pin, INPUT);
digitalWrite(pin, 1);
TASK_SLEEP(1);
prev = ReadValue();

for(;;)
{
    cur = ReadValue();

    if (cur != prev)
    {
        Trace(F("Switch\tFirst"));
        TASK_SLEEP(50);
        cur = ReadValue();
        if (cur != prev)
        {
            Trace(F("Switch\tSecond"));
            prev = cur;
            if (cur)
            {
                Trace(F("Switch\tClose "), pin);
                close->Set();
                PlaySound(sound);
            }
            else
            {
                Trace(F("Switch\tOpen "), pin);
                open->Set();
            }
        }
    }
    
    TASK_SLEEP(10);
}


TASK_BODY_END


bool ReadValue() const
{
    return digitalRead(pin);
}

byte pin;
WaitHandles::AutoResetEvent *close;
WaitHandles::AutoResetEvent *open;
SoundType sound;

public:
SwitchTask& Configure(byte pin, WaitHandles::AutoResetEvent& close, WaitHandles::AutoResetEvent& open, SoundType sound)
{
    this->pin = pin;
    this->close = &close;
    this->open = &open;
    this->sound = sound;

    

    return *this;
}

TASK_CLASS_END


void RegisterSwitchesTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<SwitchTask, 1>().Configure((uint8_t)Pins::SwitchLimits::Start, Globals::SwitchStartClose, Globals::SwitchStartOpen, SoundType::Park));
    scheduler.Register(Instance<SwitchTask, 2>().Configure((uint8_t)Pins::SwitchLimits::Finish, Globals::SwitchFinishClose, Globals::SwitchFinishOpen, SoundType::Finish));

}