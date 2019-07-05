#include <Arduino.h>
#include "Pins.h"
#include "Scheduler.h"
#include "StaticAllocActivator.h"

void setup() __ATTR_NORETURN__;

static Scheduler scheduler;

void setup()
{
    Serial.begin(115200);

    for (;;)
    {
        scheduler.Invoke();
    }
}

void loop()
{


}
