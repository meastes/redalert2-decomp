// void __fastcall Audio_Event_Type_Set_Loop(AudioEventType *type, int count) -- 0x00406650
// Purpose: stores count in the type's LoopCount setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Loop(AudioEventType *type, int count)
{
    type->LoopCount = count;
}
