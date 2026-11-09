// void __fastcall Audio_Event_Type_Set_Limit(AudioEventType *type, int limit) -- 0x004065E0
// Purpose: stores limit in the type's unknown_48 setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Limit(AudioEventType *type, int limit)
{
    type->unknown_48 = limit;
}
