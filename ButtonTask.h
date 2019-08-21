#pragma once
#include "Scheduler.h"

void RegisterButtonTask(Scheduler&);

void WaitClick(bool _long, bool _short);

void StopWaitClick();

bool CheckReset(bool& flag);
