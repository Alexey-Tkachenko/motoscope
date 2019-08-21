#include "SwitchesTask.h"
#include "ExpressTask.h"
#include "Pins.h"
#include "Globals.h"
#include "SoundTask.h"
#include "StaticAllocActivator.h"
#include "Trace.h"

#include <Arduino.h>

static const __FlashStringHelper* cat()
{
    return F("Switch");
}

TASK_BEGIN(SwitchTask, {bool cur; bool prev; bool initialized = false;})

TASK_YIELD_WHILE(initialized == false);

pinMode(pin, INPUT);
digitalWrite(pin, 1);
TASK_SLEEP(1);
prev = ReadValue();

for(;;)
{
    cur = ReadValue();

    if (cur != prev)
    {
        Trace(cat(), F("First"));
        TASK_SLEEP(50);
        cur = ReadValue();
        if (cur != prev)
        {
            Trace(cat(), F("Second"));
            prev = cur;
            if (cur)
            {
                Trace(cat(), F("Close"), pin);
                *close = true;
                PlaySound(sound);
            }
            else
            {
                Trace(cat(), F("Open"), pin);
                *open = true;
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
bool *close;
bool *open;
SoundType sound;

public:
SwitchTask& Configure(byte pin, bool& close, bool& open, SoundType sound)
{
    this->pin = pin;
    this->close = &close;
    this->open = &open;
    this->sound = sound;

    this->initialized = true;

    return *this;
}

TASK_CLASS_END


void RegisterSwitchesTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<SwitchTask, 1>().Configure((uint8_t)Pins::SwitchLimits::Start, Globals::SwitchStartClose, Globals::SwitchStartOpen, SoundType::Park));
    scheduler.Register(Instance<SwitchTask, 2>().Configure((uint8_t)Pins::SwitchLimits::Finish, Globals::SwitchFinishClose, Globals::SwitchFinishOpen, SoundType::Finish));

}