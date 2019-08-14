#include "ButtonTask.h"
#include "ExpressTask.h"
#include "StaticAllocActivator.h"
#include "Pins.h"
#include "SoundTask.h"
#include "Globals.h"
#include "Parameters.h"
#include "Trace.h"

static constexpr uint8_t _button = (uint8_t)Pins::Control::Button;

static union
{
    uint8_t any;
    struct
    {
        uint8_t longClick : 1;
        uint8_t shortClick : 1;
    };
} wait;

TASK_BEGIN(ButtonTask, { unsigned long start; })

this->Initialize();

for (;;)
{
restart:
    TASK_YIELD_WHILE(::wait.any == 0);

released:

    while (true)
    {
        if (ButtonPressed())
        {
            Trace(F("Button\tPressed"));
            start = millis();
            break;
        }
        if (!::wait.any) goto restart;
        TASK_YIELD();
    }

//state_clickStarted:
    while (start + Parameters::ClickGap > millis())
    {
        if (!::wait.any) goto restart;
        if (!ButtonPressed()) goto released;
        TASK_YIELD();
    }

    Trace(F("Button\tShortClick"));

    PlaySound(SoundType::ShortClick);
    while (start + Parameters::ClickShort > millis())
    {
        if (!::wait.any) goto waitRelease;
        if (!ButtonPressed())
        {
            if (::wait.shortClick)
            {
                PlaySound(SoundType::None);
                Trace(F("Button\tShortClick/Commited"));
                Globals::ClickShort.Set();
                goto released;
            }
        }
        TASK_YIELD();
    }

    Trace(F("Button\tLongClick"));

    PlaySound(SoundType::LongClick);
    while (start + Parameters::ClickLong > millis())
    {
        if (!::wait.any) goto waitRelease;
        if (!ButtonPressed())
        {
            if (::wait.longClick)
            {
                PlaySound(SoundType::None);
                Trace(F("Button\tLongClick/Commited"));
                Globals::ClickLong.Set();
            }
            goto released;
        }
        TASK_YIELD();
    }

waitRelease:
    Trace(F("Button\tWaitRelease"));

    PlaySound(SoundType::None);
    while (ButtonPressed())
    {
        TASK_YIELD();
    }
    Trace(F("Button\tReleased"));

}
TASK_BODY_END

static bool ButtonPressed()
{
    return !!digitalRead(_button);
}

static void Initialize()
{
    pinMode(_button, INPUT);
    digitalWrite(_button, 1);
    delay(1);
}

TASK_CLASS_END

void RegisterButtonTask(Scheduler &scheduler)
{
    scheduler.Register(Instance<ButtonTask>());
}

void WaitClick(bool _long, bool _short)
{
    ::wait.longClick = _long;
    ::wait.shortClick = _short;
}

void StopWaitClick()
{
    ::wait.any = 0;
    Globals::ClickLong.Reset();
    Globals::ClickShort.Reset();
}

