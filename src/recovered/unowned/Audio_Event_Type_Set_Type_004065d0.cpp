// void __fastcall Audio_Event_Type_Set_Type(AudioEventType *type, unsigned int flags) -- 0x004065D0
// Purpose: stores flags in the type's unknown_14 setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Type(AudioEventType *type, unsigned int flags)
{
    type->unknown_14 = flags;
}
