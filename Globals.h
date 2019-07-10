#pragma once
#include "WaitHandles.h"

namespace Globals
{
    extern WaitHandles::ValueHolder<int> CorrectionValue;

    extern WaitHandles::AutoResetEvent SwitchStartClose;
    extern WaitHandles::AutoResetEvent SwitchStartOpen;
    extern WaitHandles::AutoResetEvent SwitchFinishClose;
    extern WaitHandles::AutoResetEvent SwitchFinishOpen;

    extern WaitHandles::AutoResetEvent ClickShort;
    extern WaitHandles::AutoResetEvent ClickLong;
}
