#include "Globals.h"
namespace Globals
{
    WaitHandles::ValueHolder<int> CorrectionValue;

    WaitHandles::AutoResetEvent SwitchStartClose;
    WaitHandles::AutoResetEvent SwitchStartOpen;
    WaitHandles::AutoResetEvent SwitchFinishClose;
    WaitHandles::AutoResetEvent SwitchFinishOpen;

    WaitHandles::AutoResetEvent ClickShort;
    WaitHandles::AutoResetEvent ClickLong;
}
