#include "StepperTask.h"
#include <Arduino.h>
#include "ExpressTask.h"
#include "StaticAllocActivator.h"
#include "Globals.h"
#include "Pins.h"
#include "Trace.h"

enum class Direction : byte
{
    Undefined = 0,
    Forward,
    Backward,
    FastForward,
};

enum class StepperDirection : byte
{
    Forward = 1,
    Backward = 0,
};

static Direction direction;
static double period;
static bool periodChanged;

static const __FlashStringHelper* cat()
{
    return F("Stepper");
}

TASK_BEGIN(StepperTask, { 
    unsigned long start;
    unsigned long now;
    unsigned long wait; 
    
    unsigned long counter; 
    unsigned long tmp; 
    Direction prevDirection; 
})

pinMode((uint8_t)Pins::Stepper::Enable, OUTPUT);
pinMode((uint8_t)Pins::Stepper::Direction, OUTPUT);
pinMode((uint8_t)Pins::Stepper::Pulse, OUTPUT);

prevDirection = direction;
ResetCounter();

for (;;)
{
    if (direction == Direction::Undefined)
    {
        if (direction != prevDirection)
        {
            Trace(cat(), F("Stop"));
            prevDirection = direction;
            digitalWrite((uint8_t)Pins::Stepper::Enable, 0);
        }
        TASK_YIELD();
    }
    else if (direction == Direction::Backward)
    {
        if (direction != prevDirection)
        {
            Trace(cat(), F("Backward"));
            prevDirection = direction;
            digitalWrite((uint8_t)Pins::Stepper::Enable, 1);
            digitalWrite((uint8_t)Pins::Stepper::Direction, (uint8_t)StepperDirection::Backward);
        }
        digitalWrite((uint8_t)Pins::Stepper::Pulse, 1);
        tmp = micros();
        while (micros() - tmp < 200)
        {
            TASK_YIELD();
        }
        digitalWrite((uint8_t)Pins::Stepper::Pulse, 0);
        while (micros() - tmp < 400)
        {
            TASK_YIELD();
        }
    }
    else if (direction == Direction::FastForward)
    {
        if (direction != prevDirection)
        {
            Trace(cat(),F("FastForward"));
            prevDirection = direction;
            digitalWrite((uint8_t)Pins::Stepper::Enable, 1);
            digitalWrite((uint8_t)Pins::Stepper::Direction, (uint8_t)StepperDirection::Forward);
        }
        digitalWrite((uint8_t)Pins::Stepper::Pulse, 1);
        tmp = micros();
        while (micros() - tmp < 200)
        {
            TASK_YIELD();
        }
        digitalWrite((uint8_t)Pins::Stepper::Pulse, 0);
        while (micros() - tmp < 400)
        {
            TASK_YIELD();
        }
    }
    else if (direction == Direction::Forward)
    {
        if (direction != prevDirection)
        {
            Trace(cat(), F("Forward"));
            prevDirection = direction;
            digitalWrite((uint8_t)Pins::Stepper::Enable, 1);
            digitalWrite((uint8_t)Pins::Stepper::Direction, (uint8_t)StepperDirection::Forward);

            ResetCounter();
        }

        if (periodChanged)
        {
            Trace(cat(), F("Period"), period);
            ResetCounter();
            periodChanged = false;
        }
        else if (now = millis(), now >= wait)
        {
            
            digitalWrite((uint8_t)Pins::Stepper::Pulse, 1);
            tmp = micros();

            counter += 1;
            wait = (unsigned long)(start + counter * period + 0.5);
            
            Trace(cat(), F("Step"), wait);

            while (micros() - tmp < 200)
            {
                TASK_YIELD();
            }
            digitalWrite((uint8_t)Pins::Stepper::Pulse, 0);

        }
        TASK_YIELD();
    }
}

TASK_BODY_END

void ResetCounter()
{
    start = millis();
    counter = 1;
    wait = (unsigned long)(0.5 + start + counter * period);
}

TASK_CLASS_END

void RegisterStepperTask(Scheduler& scheduler)
{
    scheduler.Register(Instance<StepperTask>());
}

void StepperMoveForward()
{
    direction = Direction::Forward;
}

void StepperMoveBackward()
{
    direction = Direction::Backward;
}

void StepperStop()
{
    direction = Direction::Undefined;
}

void StepperFastForward()
{
    direction = Direction::FastForward;
}

void StepperSetPeriod(double period)
{
    ::period = period * 1000;
    Trace(cat(), F("Period"), ::period);
    ::periodChanged = true;
}

