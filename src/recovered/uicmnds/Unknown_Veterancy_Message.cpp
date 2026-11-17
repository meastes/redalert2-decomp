// wchar_t const *__fastcall Unknown_Veterancy_Message_006F8130(int rank) -- 0x006F8130
// The status message for a veterancy rank: 0 elite, 1 veteran, 2 little experience; empty for any other rank
// (hypothesis).
#include "shared/string_table.hpp"
extern wchar_t const unknown_empty_wide_839d4c[];

wchar_t const *__fastcall Unknown_Veterancy_Message_006F8130(int rank)
{
    if (rank == 0) {
        return StringTable::Load_String("MSG:Elite", 0, "D:\\Ra2\\UICmnds.cpp", 0x5f8);
    }
    if (rank == 1) {
        return StringTable::Load_String("MSG:Veteran", 0, "D:\\Ra2\\UICmnds.cpp", 0x5fa);
    }
    if (rank == 2) {
        return StringTable::Load_String("MSG:LittleExperience", 0, "D:\\Ra2\\UICmnds.cpp", 0x5fc);
    }
    return unknown_empty_wide_839d4c;
}
