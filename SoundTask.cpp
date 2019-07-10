#include "SoundTask.h"
#include "ExpressTask.h"
#include "StaticAllocActivator.h"
#include "Critical.h"
#include "Globals.h"
#include "Pins.h"

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
        continue;
    case SoundType::ShortClick:
        frequency = 800;
        duration = 200;
        break;
    case SoundType::LongClick:
        frequency = 700;
        duration = 500;
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
