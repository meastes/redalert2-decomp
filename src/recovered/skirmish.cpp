// skirmish.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/skirmish.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/skirmish.hpp"
#include "shared/string_table.hpp"


// Header-static dynamic initializers of the unit at 0x00682600: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00682600-0x006827E1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00682620, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x00682650, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x00682670, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x00682690, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x006826B0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x006826D0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x006826F0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x00682730, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x00682760, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x00682790, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x006827C0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// Dynamic initializer (0x006827D0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00682600.cpp; no source of its own.


// int __fastcall Skirmish_Player_Combo_ID(int index) (0x006827F0)
// The player-type combo box of player index + 1, or -1.

int __fastcall Skirmish_Player_Combo_ID(int index)
{
    if (index == 0) {
        return 0x50B;
    }
    if (index == 1) {
        return 0x50E;
    }
    if (index == 2) {
        return 0x516;
    }
    if (index == 3) {
        return 0x51A;
    }
    if (index == 4) {
        return 0x51B;
    }
    if (index == 5) {
        return 0x51C;
    }
    return index == 6 ? 0x51D : -1;
}


// int __fastcall Skirmish_Player_Index(int id) (0x00682850)
// The index (player - 1) of a player-type combo box, or -1.

int __fastcall Skirmish_Player_Index(int id)
{
    if (id == 0x50B) {
        return 0;
    }
    if (id == 0x50E) {
        return 1;
    }
    if (id == 0x516) {
        return 2;
    }
    if (id == 0x51A) {
        return 3;
    }
    if (id == 0x51B) {
        return 4;
    }
    if (id == 0x51C) {
        return 5;
    }
    if (id == 0x51D) {
        return 6;
    }
    return -1;
}


// wchar_t const *__fastcall Skirmish_Player_Type_Name(int type) (0x006828B0)
// The text of a player type (-1 none, 2 dumb, 1 smart, 0 genius computer), or null.


wchar_t const *__fastcall Skirmish_Player_Type_Name(int type)
{
    if (type == -1) {
        return StringTable::Load_String("STT:PlayerNone", 0, "D:\\Ra2\\Skirmish.cpp", 0x85);
    }
    if (type == 2) {
        return StringTable::Load_String("STT:PlayerDumbAI", 0, "D:\\Ra2\\Skirmish.cpp", 0x87);
    }
    if (type == 1) {
        return StringTable::Load_String("STT:PlayerSmartAI", 0, "D:\\Ra2\\Skirmish.cpp", 0x89);
    }
    if (type == 0) {
        return StringTable::Load_String("STT:PlayerGeniusAI", 0, "D:\\Ra2\\Skirmish.cpp", 0x8B);
    }
    return 0;
}


// bool __fastcall Skirmish_Has_Computer_After(int player) (0x00682930)
// Whether any player from `player` to 7 is a computer player.

bool __fastcall Skirmish_Has_Computer_After(int player)
{
    HWND dialog = SkirmishDialog;
    for (int index = player; index < 8; index++) {
        if (Skirmish_Is_Computer(dialog, Skirmish_Player_Combo_ID(index - 1))) {
            return true;
        }
    }
    return false;
}
