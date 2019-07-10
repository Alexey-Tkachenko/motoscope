#ifndef _SOUND_H_
#define _SOUND_H_

#include "Scheduler.h"

enum class SoundType : byte
{
    None,
    ShortClick,
    LongClick,
    Finish,
    Park,
    VelocityStep,
};


void PlaySound(SoundType type);

bool RegisterSoundTask(Scheduler& scheduler);

#endif
