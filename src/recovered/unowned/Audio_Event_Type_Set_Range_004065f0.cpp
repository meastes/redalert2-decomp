// void __fastcall Audio_Event_Type_Set_Range(AudioEventType *type, int range) -- 0x004065F0
// Purpose: stores range in the type's unknown_50 setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Range(AudioEventType *type, int range)
{
    type->unknown_50 = range;
}
