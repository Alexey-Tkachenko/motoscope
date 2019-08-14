#pragma once

#include <Arduino.h>

inline void TraceCommon(const __FlashStringHelper* str)
{
    Serial.print(millis());
    Serial.print(F(" :\t"));
    Serial.print(str);
}

inline void Trace(const __FlashStringHelper* str)
{
    TraceCommon(str);
    Serial.println();
}

template<class T>
void Trace(const __FlashStringHelper* str, const T& arg)
{
    TraceCommon(str);
    Serial.println(arg);
}
