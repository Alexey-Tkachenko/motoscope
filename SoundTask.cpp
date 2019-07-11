#include "SoundTask.h"
#include "ExpressTask.h"
#include "StaticAllocActivator.h"
#include "Critical.h"
#include "Globals.h"
#include "Pins.h"
#include "Parameters.h"

static WaitHandles::ValueHolder<SoundType> latch;

TASK_BEGIN(SoundPlayer, { SoundType r; })

for (;;)
{
	TASK_WAIT_FOR(&latch);
	{
		Critical _;
		r = latch.Get();
	}

    unsigned frequency;
    unsigned duration;

    switch (r)
    {
    case SoundType::None:
        noTone((uint8_t)Pins::Indication::Buzzer);
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

	tone((uint8_t)Pins::Indication::Buzzer, frequency, duration);
    TASK_SLEEP(100);
}

TASK_END;

void PlaySound(SoundType type)
{
    latch.Set(type);
}

bool RegisterSoundTask(Scheduler & scheduler)
{
	return scheduler.Register(Instance<SoundPlayer>());
}
