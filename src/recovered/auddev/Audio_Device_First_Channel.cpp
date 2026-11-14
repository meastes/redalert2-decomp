// AudioListNode *__fastcall Audio_Device_First_Channel(AudioDevice *device, AudioListIterator *cursor) -- 0x004035B0
// Purpose: starts a walk over the device's channel list: cursor->Node = the first channel, returned too
// (Audio_Thread_Proc 0x004094D0 reads EAX after the call; the twin of Audio_First_Device 0x004031D0).
#include "shared/audio.hpp"

AudioListNode *__fastcall Audio_Device_First_Channel(AudioDevice *device, AudioListIterator *cursor)
{
    return cursor->Node = Audio_List_First(&device->ChannelList);
}
