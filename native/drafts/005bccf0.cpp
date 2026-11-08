// NON-EXACT DRAFT 0x5BCCF0 FUN_005bccf0
// entry 0x5BCCF0; exact-match score 98.94 (misses_index best), last lane score 98.88; plan "draft as-is"
// origin lane: Scout932 (build/class-work/Scout932/keep/n1.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// int __fastcall Set_Slot_Country_Color_005bccf0(char const *name, int country, int color) (0x005BCCF0)
#include "shared/lobby.hpp"
#include "shared/gdlg_supp.hpp"
// @ref ?Unknown_005bbdd0@@YIX_N@Z=005bbdd0

int __fastcall Set_Slot_Country_Color_005bccf0(char const *name, int country, int color)
{
    int slot = -1;
    for (int i = 0; i < 8; i++) {
        char *slot_name = Unknown_006FA120(unknown_player_slots_a3fa98[i]->Name);
        if (Compare_Narrow_00781f20(slot_name, name) == 0) {
            slot = i;
            break;
        }
    }
    int changed = 0;
    if (slot == -1) {
        return 0;
    }
    HWND dialog = Find_Dialog_00735940(0xBC);
    if (dialog == 0) {
        dialog = Find_Dialog_00735940(0xBD);
    }
    NodeNameType *entry = unknown_player_slots_a3fa98[slot];
    if (entry->unknown_66e290() != country) {
        changed = 1;
        entry->unknown_66e2d0(country);
    }
    int id = Player_Side_Combo_ID_4d1350(slot);
    bool observer = Side_Combo_Data_4d1c70(dialog, id, -1) == -3;
    if (country == -3) {
        if (!observer) {
            Fill_Observer_Combo_4d1640(dialog, id);
            int control = Slot_Control_Id_005c97a0(slot);
            SendMessageA(GetDlgItem(dialog, control), 0x4F1, 0, 1);
            int color_id = Player_Color_Combo_ID_4d1cb0(slot);
            Fill_Random_Color_Combo_4d2250(dialog, color_id);
            Select_Color_Combo_4d2480(dialog, color_id, 8);
        }
    } else if (observer) {
        Fill_Side_Combo_4d1560(dialog, id);
        int control = Slot_Control_Id_005c97a0(slot);
        SendMessageA(GetDlgItem(dialog, control), 0x4F1, 0, 0);
        int color_id = Player_Color_Combo_ID_4d1cb0(slot);
        Fill_Color_Combo_4d2080(dialog, color_id);
        Select_Color_Combo_4d2480(dialog, color_id, -2);
    }
    if (unknown_player_slots_a3fa98[slot]->unknown_66e2a0() != color) {
        entry->unknown_66e300(color);
    }
    if (unknown_player_slots_a3fa98[slot] == unknown_players_a3fa7c[0]) {
        unknown_a3d3b4 = color;
    }
    Unknown_005bbdd0(false);
    return changed;
}
