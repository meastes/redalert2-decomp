// void __fastcall Audio_Event_Type_Set_FShift(AudioEventType *type, int low, int high) -- 0x00406620
// Purpose: stores the low and high ends of the type's pause between loops range (unknown_60, unknown_64).
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_FShift(AudioEventType *type, int low, int high)
{
    type->unknown_60 = low;
    type->unknown_64 = high;
}
