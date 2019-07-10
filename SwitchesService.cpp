#include "SwitchesService.h"
#include "ExpressTask.h"
#include "Pins.h"
#include "Globals.h"
#include "SoundTask.h"

static const uint8_t _start = (uint8_t)Pins::SwitchLimits::Start;
static const uint8_t _finish = (uint8_t)Pins::SwitchLimits::Finish;

static void SwitchStartInterrupt()
{
    bool value = !digitalRead(_start);
    (value ? Globals::SwitchStartClose : Globals::SwitchStartOpen).Set();
    if (value)
    {
        PlaySound(SoundType::Park);
    }
}

static void SwitchFinishInterrupt()
{
    bool value = !digitalRead(_finish);
    (value ? Globals::SwitchFinishClose : Globals::SwitchFinishOpen).Set();
    if (value)
    {
        PlaySound(SoundType::Finish);
    }
}

void RegisterSwitchesService()
{
    pinMode(_start, INPUT);
    digitalWrite(_start, 1);

    pinMode(_finish, INPUT);
    digitalWrite(_finish, 1);

    delay(1);

    attachInterrupt(digitalPinToInterrupt(_start), SwitchStartInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_finish), SwitchFinishInterrupt, CHANGE);
}