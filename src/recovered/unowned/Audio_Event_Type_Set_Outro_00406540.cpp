// void __fastcall Audio_Event_Type_Set_Outro(AudioEventType *type, int count) -- 0x00406540
// Purpose: stores count in the type's Outro setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Outro(AudioEventType *type, int count)
{
    type->Outro = count;
}
