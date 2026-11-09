// void __fastcall Audio_Event_Type_Set_Intro(AudioEventType *type, int count) -- 0x00406530
// Purpose: stores count in the type's Intro setting.
#include "shared/audio_event.hpp"

void __fastcall Audio_Event_Type_Set_Intro(AudioEventType *type, int count)
{
    type->Intro = count;
}
