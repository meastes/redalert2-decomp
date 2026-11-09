// void __fastcall Audio_Event_Type_Set_Min_Volume(AudioEventType *type, float volume) -- 0x00406600
// Purpose: stores volume in the type's unknown_54 setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Min_Volume(AudioEventType *type, float volume)
{
    type->unknown_54 = volume;
}
