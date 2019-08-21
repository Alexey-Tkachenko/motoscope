#pragma once

#include "ExpressTask.h"

void RegisterLedTask(Scheduler& scheduler);

void LedSetValue(int value);

void LedLowPowerStart();
void LedLowPowerEnd();
