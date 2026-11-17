// void Audio_Destroy_Drivers() -- 0x00402BB0
// Purpose: destroys every driver instance in the system's driver list
// (Audio_Destroy_Driver expanded inline; compiled in one unit with it).
#include "shared/audio.hpp"

void Audio_Destroy_Drivers()
{
    AudioListNode *next = Audio_List_First(&Audio_System_Instance->Drivers);
    for (AudioListNode *node = next; node != 0; node = next) {
        next = Audio_List_Next(next);
        Audio_Destroy_Driver((AudioDriver *)node);
    }
}
