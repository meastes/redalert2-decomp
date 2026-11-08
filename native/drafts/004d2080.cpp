// NON-EXACT DRAFT 0x4D2080 FUN_004d2080
// entry 0x4D2080; exact-match score 86.09 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout1018 (build/class-work/Scout1018/keep/fill_color_combo_4d2080_fc1_8609.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// void __fastcall Fill_Color_Combo_4d2080(HWND dialog, int id) (0x004D2080)
// Fills a player row's colour combo box with "GUI:RandomAsSymbols" (item data -2)
// and every colour table entry that is free or held by this row's player (item
// data: the entry), selects the row's own colour (or the random item) and shows
// the box again when it was visible.
#include "shared/gdlg_supp.hpp"
#include "shared/string_table.hpp"
// @ref __imp__GetDlgItem@8=0079a49c
// @ref __imp__ShowWindow@8=0079a498
// @ref __imp__SendMessageA@16=0079a4a0
// @ref ??_C@_0BE@MIDK@GUI?3RandomAsSymbols?$AA@=007d87dc

void __fastcall Fill_Color_Combo_4d2080(HWND dialog, int id)
{
    HWND combo = GetDlgItem(dialog, id);
    bool hidden = ShowWindow(combo, SW_HIDE) == 0;
    SendMessageA(combo, CB_RESETCONTENT, 0, 0);
    SendMessageA(combo, 0x4DD, 0, 1);
    SendMessageA(combo, 0x4DE, 0, 9);
    int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::Load_String("GUI:RandomAsSymbols", 0, "D:\\Ra2\\GDlgSupp.cpp", 502));
    SendMessageA(combo, CB_SETITEMDATA, item, -2);
    SendMessageA(combo, 0x498, item, -1);
    int player = Color_Combo_Player_4d1d10(id);
    int selected = 0;
    for (int index = 0; index < 8; index++) {
        bool mine = unknown_color_entries_8660d8[index].Index == player;
        if (mine || unknown_color_entries_8660d8[index].Index == -1) {
            item = SendMessageA(combo, 0x4C2, 0, (LPARAM)unknown_color_entries_8660d8[index].Name);
            SendMessageA(combo, 0x498, item, unknown_color_entries_8660d8[index].Color);
            SendMessageA(combo, CB_SETITEMDATA, item, index);
            if (mine) {
                selected = item;
            }
        }
    }
    SendMessageA(combo, CB_SETCURSEL, selected, 0);
    SendMessageA(combo, 0x4F1, 0, 0);
    if (!hidden) {
        ShowWindow(combo, SW_SHOW);
    }
}
