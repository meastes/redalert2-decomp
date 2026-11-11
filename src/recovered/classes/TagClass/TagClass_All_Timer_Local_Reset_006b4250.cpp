// void __fastcall TagClass::All_Timer_Local_Reset(int value) (0x006B4250)
// Passes value to the first trigger of every tag.
#include "shared/object.hpp"

void __fastcall TagClass::All_Timer_Local_Reset(int value)
{
    for (int index = 0; index < Tags.Count(); index++) {
        Tags[index]->Trigger->unknown_func_6ed650(value);
    }
}
