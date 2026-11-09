// void __fastcall Audio_Event_Type_Set_Priority(AudioEventType *type, int priority) -- 0x00406550
// Purpose: stores priority in the type's unknown_40 setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Priority(AudioEventType *type, int priority)
{
    type->unknown_40 = priority;
}
