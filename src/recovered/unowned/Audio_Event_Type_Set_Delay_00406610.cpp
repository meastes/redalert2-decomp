// void __fastcall Audio_Event_Type_Set_Delay(AudioEventType *type, int low, int high) -- 0x00406610
// Purpose: stores the low and high ends of the type's delay range (unknown_58, unknown_5c).
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Delay(AudioEventType *type, int low, int high)
{
    type->unknown_58 = low;
    type->unknown_5c = high;
}
