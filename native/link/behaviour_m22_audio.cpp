// Behaviour-written bodies for native M22 (Scout1177): audio, message list and
// movie leaves under reached callers. NOT EXACT: each follows its pass-6a decompile.
#define MESSAGE_LIST_DECLARE_EDIT_BUF
// 0x004052F0: sets the event's flag 8 (Voc_Update_At).
#include "shared/audio.hpp"
#include "shared/voc_class.hpp"
void __fastcall Audio_Event_Set_Flag_8(AudioEvent *event)
{
    event->Flags |= 8;
}

// 0x005B55E0 (Westwood's Get_Edit_Buf): the edit text after the 'to' prefix.
#include "shared/message_list.hpp"
unsigned short *MessageListClass::Get_Edit_Buf()
{
    return EditBuf + EditInitPos;
}

// 0x005B69E0: frees the copied name (operator delete 0x007825E8, the CRT's free).
#include "shared/ms_engine.hpp"
#include <stdlib.h>
MSSfx::~MSSfx()
{
    if (Name) {
        free(Name);
    }
}
