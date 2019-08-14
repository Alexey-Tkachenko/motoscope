#include "SoundTask.h"
#include "ExpressTask.h"
#include "StaticAllocActivator.h"
#include "Critical.h"
#include "Pins.h"
#include "Parameters.h"
#include "Trace.h"

static WaitHandles::ValueHolder<SoundType> latch;

volatile unsigned long shutdownTime;

static bool CheckTime()
{
    return millis() >= shutdownTime;
}

static WaitHandles::Condition shutdown(CheckTime);


TASK_BEGIN(SoundPlayer, { SoundType r; })

for (;;)
{
	TASK_WAIT_FOR(&latch);
	{
		Critical _;
		r = latch.Get();
	}

    Trace(F("Sound\tPlay "), (int)r);

    unsigned frequency;
    unsigned duration;

    switch (r)
    {
    case SoundType::None:
        shutdownTime = 0;
        continue;
    case SoundType::StartClick:
        frequency = 900;
        duration = Parameters::ClickGap;
        break;
    case SoundType::ShortClick:
        frequency = 800;
        duration = Parameters::ClickShort;
        break;
    case SoundType::LongClick:
        frequency = 700;
        duration = Parameters::ClickLong;
        break;
    case SoundType::Finish:
        frequency = 1000;
        duration = 2000;
        break;
    case SoundType::Park:
        frequency = 600;
        duration = 2000;
        break;
    case SoundType::VelocityStep:
        frequency = 200;
        duration = 50;
        break;
    case SoundType::PowerAlarm1:
        frequency = 2500;
        duration = 500;
        break;
    case SoundType::PowerAlarm2:
        frequency = 3000;
        duration = 500;
        break;
    default:
        continue;
    }

    Trace(F("Sound\tSetBuzzer"));
	tone((uint8_t)Pins::Indication::Buzzer, frequency);
    shutdownTime = millis() + duration;
    TASK_SLEEP(100);
}

TASK_END;

TASK_BEGIN(SoundShutdownTask, {})
for(;;)
{
    TASK_WAIT_FOR(&shutdown);
    shutdownTime = -1;
    Trace(F("Sound\tShutdown"));
    noTone((uint8_t)Pins::Indication::Buzzer);
}
TASK_END

void PlaySound(SoundType type)
{
    Trace(F("Sound\tLatch "), (int)type);
    latch.Set(type);
}

bool RegisterSoundTask(Scheduler & scheduler)
{
	return scheduler.Register(Instance<SoundPlayer>()) && scheduler.Register(Instance<SoundShutdownTask>());
}
