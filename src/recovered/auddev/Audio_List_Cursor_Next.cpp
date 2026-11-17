// AudioListNode *__fastcall Audio_List_Cursor_Next(AudioListCursor *cursor) -- 0x00402C20
// Purpose: the out-of-line copy of AudioListCursor::Next: advances the
// cursor when it has a node and returns its node.
#include "shared/audio.hpp"

AudioListNode *__fastcall Audio_List_Cursor_Next(AudioListCursor *cursor)
{
    return cursor->Next();
}
