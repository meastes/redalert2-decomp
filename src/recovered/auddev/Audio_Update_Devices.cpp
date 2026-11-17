// void Audio_Update_Devices() -- 0x00402900
// Purpose: once the system is initialized and not busy, runs the update
// function (+0x10C) of every device in the system's device list that does
// not have flag 2 set.
#include "shared/audio.hpp"

void Audio_Update_Devices()
{
    if (Audio_Initialized != 0 && Audio_System_Instance->Busy == 0) {
        AudioListNode *list = &Audio_System_Instance->Devices;
        for (AudioDevice *device = (AudioDevice *)list->Next; device != list; device = (AudioDevice *)device->Next) {
            if (!(device->Flags & 2)) {
                device->unknown_update_10c(device);
            }
        }
    }
}
