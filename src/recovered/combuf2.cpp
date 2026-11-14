// combuf2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/combuf2.json carries the addresses and the rows that
// stay in their own files.


#include "shared/command.hpp"


// Tests modified-key acceptance through the existing CommandClass slot5 predicate.
bool __fastcall Command_Allows_Modified_Key_00480b20(CommandClass const &command,unsigned int key)
{
    key &= 0xfffff7ff; // remove the key-release flag
    if (key!=(key & 0xffffe8ff)) return !command.unknown_slot5(key);
    return true;
}


bool __fastcall Command_Allows_Key_00480b40(unsigned int key)
{
    unsigned int normalized = key & 0xfffff7ff; // remove the key-release flag
    CommandClass *command = unknown_hotkeys_832380[(int)(normalized & 0xffffe8ff)];
    if (command == 0) return true;
    return !command->unknown_slot5(normalized);
}


// Signed int-key qsort comparator emitted by the existing IndexClass<int, CommandClass *> lookup.
