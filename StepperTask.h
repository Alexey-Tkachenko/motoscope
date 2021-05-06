#pragma once
#include "Scheduler.h"

void RegisterStepperTask(Scheduler&);

void StepperMoveForward();

void StepperMoveBackward();

void StepperFastForward();

void StepperStop();

void StepperSetPeriod(double period);
