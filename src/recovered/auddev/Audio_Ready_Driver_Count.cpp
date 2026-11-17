// int Audio_Ready_Driver_Count() -- 0x00402C40
// Purpose: counts the ready drivers: sums their +0x18 flags over the
// system's driver list.
#include "shared/audio.hpp"

int Audio_Ready_Driver_Count()
{
    int total = 0;
    AudioListCursor cursor;
    cursor.First(&Audio_System_Instance->Drivers);
    for (AudioListNode *node = cursor.Node; node != 0; node = cursor.Next()) {
        total += ((AudioDriver *)node)->unknown_ready_18;
    }
    return total;
}
